// Google Map
let map;

// Markers for map
let markers = [];

// Info window
let info = new google.maps.InfoWindow();


// Execute when the DOM is fully loaded
$(document).ready(function() {

    // Styles for map
    // https://developers.google.com/maps/documentation/javascript/styling
    let styles = [

        // Hide Google's labels
        {
            featureType: "all",
            elementType: "labels",
            stylers: [
                { visibility: "off" }
            ]
        },

        // Hide roads
        {
            featureType: "road",
            elementType: "geometry",
            stylers: [
                { visibility: "off" }
            ]
        }

    ];

    // Options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    let options = {
        center: { lat: 42.3770, lng: -71.1256 }, // Cambridge
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,
        panControl: true,
        styles: styles,
        zoom: 13,
        zoomControl: true
    };

    // Get DOM node in which map will be instantiated
    let canvas = $("#map-canvas").get(0);

    // Instantiate map
    map = new google.maps.Map(canvas, options);

    // Configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);

});


// Add marker for place to map
function addMarker(place) {
    //instantiate - google.maps.Marker
    var marker = new google.maps.Marker({
        position: { lat: place.latitude, lng: place.longitude },
        map: map,
        title: place.place_name + ", " + place.admin_name1,
        label: place.place_name + ", " + place.admin_name1
    });
    //listen for clicks on Marker - google.maps.event.addListener
    //retrieve articles on click
    marker.addListener('click', function() {
        var titles = ["", "", "", "", ""];
        var links = ["", "", "", "", ""];

        //get articles for place zip and store into array - $.getJSON
        $.getJSON("/articles?geo=" + place.postal_code, function(addr) {
            //use if statements to avoid possible index errors
            // console.log("Value of addr.length: ");
            // console.log(addr.length);
            if (addr.length >= 1) {
                //update titles and links array here
                //console.log("Entered first if!");
                titles[0] = addr[0].title;
                links[0] = addr[0].link;
                // console.log("updated value of titles[0]:");
                // console.log(titles[0]);
            }
            if (addr.length >= 2) {
                //update titles and links array here
                titles[1] = addr[1].title;
                links[1] = addr[1].link;
            }
            if (addr.length >= 3) {
                //update titles and links array here
                titles[2] = addr[2].title;
                links[2] = addr[2].link;
            }
            if (addr.length >= 4) {
                //update titles and links array here
                titles[3] = addr[3].title;
                links[3] = addr[3].link;
            }
            if (addr.length >= 5) {
                //update titles and links array here
                titles[4] = addr[4].title;
                links[4] = addr[4].link;
            }

            //build list of links to articles
            var contentString = '<ul>' +
                '<li>' + '<a href ="' + links[0] + '">' + titles[0] + '</a>' + '</li>' +
                '<li>' + '<a href ="' + links[1] + '">' + titles[1] + '</a>' + '</li>' +
                '<li>' + '<a href ="' + links[2] + '">' + titles[2] + '</a>' + '</li>' +
                '<li>' + '<a href ="' + links[3] + '">' + titles[3] + '</a>' + '</li>' +
                '<li>' + '<a href ="' + links[4] + '">' + titles[4] + '</a>' + '</li>' +
                '</ul>';

            //create info window
            var infowindow = new google.maps.InfoWindow({
                content: contentString
            });

            //open info window on click event
            infowindow.open(map, marker);
        });
    });

    //remember marker by adding to markers[] array
    markers.push(marker);
    // console.log("Printing markers array after pushing...")
    // console.log(markers);
}


// Configure application
function configure() {
    // Update UI after map has been dragged
    google.maps.event.addListener(map, "dragend", function() {

        // If info window isn't open
        // http://stackoverflow.com/a/12410385
        if (!info.getMap || !info.getMap()) {
            update();
        }
    });

    // Update UI after zoom level changes
    google.maps.event.addListener(map, "zoom_changed", function() {
        update();
    });

    // Configure typeahead
    $("#q").typeahead({
        highlight: false,
        minLength: 1
    }, {
        display: function(suggestion) { return null; },
        limit: 10,
        source: search,
        templates: {
            suggestion: Handlebars.compile(
                "<div>" +
                "{{place_name}}, {{admin_name1}}, {{postal_code}}" +
                "</div>"
            )
        }
    });

    // Re-center map after place is selected from drop-down
    $("#q").on("typeahead:selected", function(eventObject, suggestion, name) {

        // Set map's center
        map.setCenter({ lat: parseFloat(suggestion.latitude), lng: parseFloat(suggestion.longitude) });

        // Update UI
        update();
    });

    // Hide info window when text box has focus
    $("#q").focus(function(eventData) {
        info.close();
    });

    // Re-enable ctrl- and right-clicking (and thus Inspect Element) on Google Map
    // https://chrome.google.com/webstore/detail/allow-right-click/hompjdfbfmmmgflfjdlnkohcplmboaeo?hl=en
    document.addEventListener("contextmenu", function(event) {
        event.returnValue = true;
        event.stopPropagation && event.stopPropagation();
        event.cancelBubble && event.cancelBubble();
    }, true);

    // Update UI
    update();

    // Give focus to text box
    $("#q").focus();
}


// Remove markers from map
function removeMarkers() {
    // call the setMap() method passing null as the argument
    // Example: marker.setMap(null);
    // If you want to delete the marker, set the marker itself to null

    //iterate through markers array set them
    for (var i = 0; i < markers.length; i++) {
        markers[i].setMap(null);
    }

    //remove references to markers
    markers = [];
}


// Search database for typeahead's suggestions
function search(query, syncResults, asyncResults) {
    // Get places matching query (asynchronously)
    let parameters = {
        q: query
    };
    $.getJSON("/search", parameters, function(data, textStatus, jqXHR) {

        // Call typeahead's callback with search results (i.e., places)
        asyncResults(data);
    });
}


// Show info window at marker with content
function showInfo(marker, content) {
    // Start div
    let div = "<div id='info'>";
    if (typeof(content) == "undefined") {
        // http://www.ajaxload.info/
        div += "<img alt='loading' src='/static/ajax-loader.gif'/>";
    } else {
        div += content;
    }

    // End div
    div += "</div>";

    // Set info window's content
    info.setContent(div);

    // Open info window (if not already open)
    info.open(map, marker);
}


// Update UI's markers
function update() {
    // Get map's bounds
    let bounds = map.getBounds();
    let ne = bounds.getNorthEast();
    let sw = bounds.getSouthWest();

    // Get places within bounds (asynchronously)
    let parameters = {
        ne: `${ne.lat()},${ne.lng()}`,
        q: $("#q").val(),
        sw: `${sw.lat()},${sw.lng()}`
    };
    $.getJSON("/update", parameters, function(data, textStatus, jqXHR) {
        // Remove old markers from map
        removeMarkers();

        // Add new markers to map
        for (let i = 0; i < data.length; i++) {
            addMarker(data[i]);
        }
    });
};
