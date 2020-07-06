import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash
from helpers import apology, login_required, lookup, usd
import re

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Retrieve every entryin in holdings with current userid
    portfolio = db.execute("SELECT * FROM holdings WHERE identity = :identity", identity=session["user_id"])

    # Retrieve user's cash and convert to USD currency
    youngmoney = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    bmoney = youngmoney[0]['cash']
    bmoney = usd(bmoney)

    # Render template index.html
    return render_template("index.html", stocks=portfolio, moolah=bmoney)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Check if symbol box is empty
        if not request.form.get("symbol"):
            return apology("Missing symbol", 400)

        # Check if shares box is empty
        if not request.form.get("shares"):
            return apology("Missing shares", 400)

        # Retrieve stock quote
        quote = lookup(request.form.get("symbol"))

        # Verify stock is valid. If not return another apology
        if quote is None:
            return apology("Invalid symbol", 400)

        # Retrieve shares and price of stock into variables
        buying = request.form.get("shares")
        cost = quote['price']

        # Verify number of shares is a natural number.
        if not buying.isdigit():
            return apology("Shares must be a natural number", 400)

        # Verify number of shares bought is atleast 1 (HTML form SHOULD already do this)
        if int(buying) < 0:
            return apology("Shares must be a positive number", 400)

    # Add stock to user's portfolio:

        # Calculate total cost (shares bought * price)
        total = int(buying) * cost

        # Retrieve user's cash
        youngmoney = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        bmoney = youngmoney[0]['cash']

        # Check if user can afford it
        remain = bmoney - total

        if remain < 0:
            return apology("Can't afford", 400)

        # Update users cash
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=remain, id=session["user_id"])

        # Convert numbers to currency before inserting into table... (is this necessary?)
        costUSD = usd(cost)
        totalUSD = usd(total)

        # Insert transaction into history table and mark it with user id...
        db.execute("INSERT INTO 'transactions' (identity, symbol, company, shares, price)\
        VALUES(:identity, :symbol, :company, :shares, :price)", identity=session["user_id"], symbol=quote['symbol'], company=quote['name'], shares=int(buying), price=costUSD)

        # Check if symbol is already present inside user portfolio
        sym = quote['symbol']
        present = db.execute("SELECT symbol FROM 'holdings' WHERE\
        identity = :identity AND symbol = :symbol", identity=session["user_id"], symbol=sym)

        # If it is NOT present, INSERT into the holdings table
        if not present:
            db.execute("INSERT INTO 'holdings' (identity, symbol, name, shares, price, total) VALUES \
            (:identity, :symbol, :name, :shares, :price, :total)", identity=session["user_id"], symbol=sym, name=quote['name'], shares=int(buying), price=costUSD, total=totalUSD)

        # If it is UPDATE the number of shares, price and total
        else:
            # Update number of shares. Retrieve number of shares and add shares bought.
            old_shares = db.execute("SELECT shares FROM holdings WHERE identity = :identity \
            AND symbol = :symbol", identity=session["user_id"], symbol=sym)

            total_shares = old_shares[0]['shares'] + int(buying)

            db.execute("UPDATE 'holdings' SET shares = :shares WHERE identity = :identity AND \
            symbol = :symbol", shares=total_shares, identity=session["user_id"], symbol=sym)

            # Update price with current price of stock. (Discarding the old one. May need to change.)
            db.execute("UPDATE 'holdings' SET price = :price WHERE identity = :identity AND \
            symbol = :symbol", price=costUSD, identity=session["user_id"], symbol=sym)

            # Retrieve old total and add the new one BEFORE updating total
            old_total = db.execute("SELECT total FROM holdings WHERE identity = :identity \
            AND symbol = :symbol", identity=session["user_id"], symbol=sym)

            # Convert total from string to float
            boom = old_total[0]['total']
            value = boom.strip("$")
            value2 = value.replace(",", '')
            old_total = float(value2)

            # Add old_price with new total and convert to USD string
            new_total = old_total + total
            new_totalUSD = usd(new_total)

            # Finally, update the holdings table with the new total price for this user.
            db.execute("UPDATE 'holdings' SET total = :total WHERE identity = :identity AND \
            symbol = :symbol", total=new_totalUSD, identity=session["user_id"], symbol=sym)

        # Return user to homepage where they will see their portfolio
        # Should also display an alert that says 'Bought!' so users knows it worked (Optional?)
        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Retrieve every entryin in holdings with current userid
    history = db.execute("SELECT * FROM transactions WHERE identity = :identity", identity=session["user_id"])

    # Render template index.html
    return render_template("history.html", stocks=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Check if symbol box is empty
        if not request.form.get("symbol"):
            return apology("Missing symbol", 400)

        # Retrieve stock quote
        quote = lookup(request.form.get("symbol"))

        # Verify stock is valid. If not return another apology
        if quote is None:
            return apology("Invalid symbol", 400)

        # Display stock quote
        return render_template("quoted.html", company=quote['name'], symbol=quote['symbol'], price=quote['price'])

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation password was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # Verify password contains at least one letter, number, and symbol
        pass1 = request.form.get("password")
        letter = re.search('[a-zA-Z]', pass1)
        number = re.search('[0-9]', pass1)
        symbol = re.search(r'[!"#\$%&\'\(\)\*\+,-\.\/:;<=>\?@[\]\^_`\{\|}~\\]', pass1)
        # Reference for regular expression in the above line: https://techearl.com/regular-expressions/regex-password-strength-validation

        if letter is None or number is None or symbol is None:
            return apology("password must contain atleast one letter, number, and symbol", 400)

        # compare first and second passwords
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 400)

        # Hash Password and prepare to insert user into database
        hashfunc = generate_password_hash(pass1)

        # Insert user into database
        insert = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)",
                            username=request.form.get("username"), hash=hashfunc)

        # If username exists return apology
        if not insert:
            return apology("Username already exists", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Check if symbol box is empty
        if not request.form.get("symbol"):
            return apology("Missing symbol", 400)

        # Check if shares box is empty
        if not request.form.get("shares"):
            return apology("Missing shares", 400)

        # Retrieve stock quote
        quote = lookup(request.form.get("symbol"))

        # Verify stock is valid. If not return another apology
        if quote is None:
            return apology("Invalid symbol", 400)

        # Retrieve shares and price of stock into variables
        selling = request.form.get("shares")
        cost = quote['price']

        # Verify number of shares is a natural number.
        if not selling.isdigit():
            return apology("Shares must be a natural number", 400)

        # Verify number of shares bought is atleast 0 (HTML form SHOULD already do this)
        if int(selling) < 0:
            return apology("Shares must be a positive number", 400)

        # Verify user actually has enough shares to sell
        share_count = db.execute("SELECT shares FROM holdings WHERE identity = :identity \
        AND symbol = :symbol", identity=session["user_id"], symbol=quote['symbol'])

        if not share_count:
            return apology("user does not have shares of this stock", 400)

        share_count = share_count[0]['shares']

        if int(selling) > share_count:
            return apology("too many shares", 400)

        # Calculate profit after selling shares
        profit = cost * int(selling)

        # Update cash with value of sold shares. Start by retrieving cash then adding.
        youngmoney = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        bmoney = youngmoney[0]['cash']

        new_money = profit + bmoney

        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=new_money, id=session["user_id"])

        # Remove stock from user's portfolio and update shares and total
        updated_shares = share_count - int(selling)

        db.execute("UPDATE holdings SET shares = :shares WHERE \
        identity = :identity AND symbol = :symbol", shares=updated_shares, identity=session["user_id"], symbol=quote['symbol'])

        old_total = db.execute("SELECT total FROM holdings WHERE identity = :identity \
        AND symbol = :symbol", identity=session["user_id"], symbol=quote['symbol'])

        boom = old_total[0]['total']
        value = boom.strip("$")
        value2 = value.replace(",", '')
        old_total = float(value2)

        new_total = old_total - profit
        new_totalUSD = usd(new_total)

        db.execute("UPDATE holdings SET total = :total WHERE \
        identity = :identity AND symbol = :symbol", total=new_totalUSD, identity=session["user_id"], symbol=quote['symbol'])

        # Check if number of shares becomes 0. If it is, delete row.
        zero_check = db.execute("SELECT shares FROM holdings WHERE identity = :identity \
        AND symbol = :symbol", identity=session["user_id"], symbol=quote['symbol'])

        zero_check = zero_check[0]['shares']

        if zero_check == 0:
            db.execute("DELETE FROM holdings WHERE identity = :identity AND symbol = :symbol",
                       identity=session["user_id"], symbol=quote['symbol'])

        # Insert into transaction table with negative share quantity
        new_moneyUSD = usd(new_money)
        shares_sold = int(selling) * (-1)

        db.execute("INSERT INTO 'transactions' (identity, symbol, company, shares, price) VALUES(:identity, :symbol, :company, :shares, :price)",
                   identity=session["user_id"], symbol=quote['symbol'], company=quote['name'], shares=shares_sold, price=new_moneyUSD)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Retrieve symbols from users portfolio
        all_symbols = db.execute("SELECT symbol FROM holdings WHERE identity = :identity", identity=session["user_id"])

        return render_template("sell.html", stocks=all_symbols)


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
