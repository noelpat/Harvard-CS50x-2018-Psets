import os
import re
from flask import Flask, jsonify, render_template, request

from cs50 import SQL
from helpers import lookup

# Configure application
app = Flask(__name__)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
def index():
    """Render map"""
    return render_template("index.html")


@app.route("/articles")
def articles():
    """Look up articles for geo"""

    if request.method == "GET":
        # geo is passed into /articles as a GET parameter
        current_geo = request.args.get("geo")
        print("Value of current geo: ")
        print(current_geo)

        # check for missing argument
        if current_geo is None:
            return jsonify([])

        # check if URL contains & because it will mess this up.
        current_geo = current_geo.replace('&', '')

        # Call lookup function from helpers
        current = lookup(current_geo)

        # print("Made it in the articles method mayne!!!")
        return jsonify(current)

    return([])


@app.route("/search")
def search():
    """Search for places that match query"""

    # q is submitted to /search as a GET parameter
    if request.method == "GET":
        # check if query only contains numbers here etc...
        checker = request.args.get("q")
        onlyNum = False
        # Check if q only contains numbers
        if checker.isdigit():
            onlyNum = True

        # grab search value with special operator behind for searching partial results
        q = request.args.get("q") + "%"

        # Query database
        # if only numbers in query...
        if onlyNum == True:
            retrieve = db.execute("SELECT * FROM places WHERE postal_code LIKE :q", q=q)
            return jsonify(retrieve)
        elif len(checker) == 2:
            # check by country code and admin_code1
            retrieve = db.execute("SELECT * FROM places WHERE admin_code1 LIKE :q OR\
            country_code LIKE :q", q=q)
            return jsonify(retrieve)
        else:
            # search by place name/admin_code1/admin_name1/admin_name2 (city,state,zip)
            q = "%" + request.args.get("q") + "%"

            # replace commas, periods and dashes in original input with spaces
            re_spec = checker.replace(',', ' ')
            re_spec = re_spec.replace('.', ' ')
            re_spec = re_spec.replace('-', ' ')

            # while loop to replace all double spaces
            # Based on idea posted from 'Marjan' at https://stackoverflow.com/questions/761804/how-do-i-trim-whitespace-from-a-python-string
            space = False
            while space == False:
                pos = re_spec.find("  ")
                if pos != -1:
                    re_spec = re_spec.replace("  ", " ")
                else:
                    space = True

            # split original input by spaces
            store = re_spec.split(" ")

            # assign the first 4 places to variables a,b,c,d if applicable
            a = ""
            b = ""
            c = ""
            d = ""

            if len(store) > 0:
                a = "%" + store[0] + "%"

            if len(store) > 1:
                b = "%" + store[1] + "%"

            if len(store) > 2:
                c = "%" + store[2] + "%"

            if len(store) > 3:
                d = "%" + store[3] + "%"

            # check for a in database for place_name/admin_name1/admin_name2
            if len(store) == 1:
                # print("made it inside first if!")
                retrieve = db.execute("SELECT * FROM places WHERE place_name LIKE :a OR\
                admin_name1 LIKE :a OR admin_name2 LIKE :a", a=a)
                return jsonify(retrieve)

            # check for a and b in the database
            if len(store) == 2:
                # check if the two words refer to one place like 'New Haven' or 'New London'
                test_place = db.execute("SELECT * FROM places WHERE place_name LIKE :q OR\
                admin_name2 LIKE :q", q=q)

                # If test_place found nothing...
                if not test_place:
                    if len(store[1]) == 2:
                        retrieve = db.execute("SELECT * FROM places WHERE (place_name LIKE :a\
                        OR admin_name1 LIKE :a OR admin_name2 LIKE :a) AND (admin_code1 LIKE :b OR\
                        country_code LIKE :b OR postal_code LIKE :b)", a=a, b=b)
                        return jsonify(retrieve)
                    else:
                        retrieve = db.execute("SELECT * FROM places WHERE place_name LIKE :a AND\
                        (admin_name1 LIKE :b OR postal_code LIKE :b)", a=a, b=b)
                        return jsonify(retrieve)
                # test_place found something. Return test_place immediately.
                else:
                    return jsonify(test_place)

            # check for a, b, and c in the database
            if len(store) > 2:
                # check if the two words refer to one place like 'New Haven' or 'New London'
                # strip out the last word (c) to check for the first two...
                combine = "%" + a + " " + b + "%"
                test_place = db.execute("SELECT * FROM places WHERE place_name LIKE :q OR\
                admin_name2 LIKE :q", q=combine)

                # If test_place found nothing...
                if not test_place:
                    if len(store[1]) == 2:
                        retrieve = db.execute("SELECT * FROM places WHERE (place_name LIKE :a\
                        OR admin_name1 LIKE :a OR admin_name2 LIKE :a) AND (admin_code1 LIKE\
                        :b) AND (country_code LIKE :c OR postal_code LIKE :c)", a=a, b=b, c=c)
                        return jsonify(retrieve)
                    else:
                        retrieve = db.execute("SELECT * FROM places WHERE (place_name LIKE :a) AND\
                        (admin_name1 LIKE :b OR admin_code1 LIKE :b)\
                        AND (country_code LIKE :c OR postal_code LIKE :c)", a=a, b=b, c=c)
                        return jsonify(retrieve)
                # test_place found something. Also account for c instead of b
                else:
                    retrieve = db.execute("SELECT * FROM places WHERE (place_name LIKE :q) AND\
                    (admin_name1 LIKE :c OR admin_code1 LIKE :c OR postal_code LIKE :c)", q=combine, c=c)
                    return jsonify(retrieve)

            # # check for a, b, c, and d in the database
            # if len(store) > 3:
            #     retrieve = db.execute("SELECT * FROM places WHERE place_name LIKE :a OR\
            #     admin_name1 LIKE :a OR admin_name2 LIKE :a", a=a)
            #     return jsonify(retrieve)

    # return empty array
    return jsonify([])


@app.route("/update")
def update():
    """Find up to 10 places within view"""

    # Ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # Ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # Explode southwest corner into two variables
    sw_lat, sw_lng = map(float, request.args.get("sw").split(","))

    # Explode northeast corner into two variables
    ne_lat, ne_lng = map(float, request.args.get("ne").split(","))

    # Find 10 cities within view, pseudorandomly chosen if more within view
    if sw_lng <= ne_lng:

        # Doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM places
                          WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
                          GROUP BY country_code, place_name, admin_code1
                          ORDER BY RANDOM()
                          LIMIT 10""",
                          sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # Crosses the antimeridian
        rows = db.execute("""SELECT * FROM places
                          WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
                          GROUP BY country_code, place_name, admin_code1
                          ORDER BY RANDOM()
                          LIMIT 10""",
                          sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # Output places as JSON
    return jsonify(rows)
