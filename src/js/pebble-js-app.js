Pebble.addEventListener("ready",
    function(e) {
        console.log("js app inited");
    }
);

Pebble.addEventListener("appmessage",
    function(e) {
        console.log("Got settings: " + JSON.stringify(e.payload));
        window.localStorage.options = JSON.stringify(e.payload);
    }
);

Pebble.addEventListener("showConfiguration",
    function(e) {
        console.log('Showing config')
        
        var url = 'http://www.somehost.com/'
        var options = window.localStorage.getItem("options")
        if (options !== null) {
            Pebble.openURL(url + '?options=' + escape(options));
        } else {
            Pebble.openURL(url);
        }
    }
);

Pebble.addEventListener("webviewclosed",
    function(e) {
        console.log('Got response: ' + e.response)
        var config = JSON.parse(e.response);
        
        window.localStorage.options = JSON.stringify(config)
        Pebble.sendAppMessage(config);
    }
);

 
