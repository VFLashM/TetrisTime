Pebble.addEventListener("ready",
    function(e) {
        console.log("js app inited");
        if (window.localStorage.getItem("options") === null) {
            Pebble.sendAppMessage({},
                                  function(e) {},
                                  function(e) {
                                      console.log('Error sending message to watch: ' + JSON.stringify(e));
                                  });
        }
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
        var url = 'https://www.googledrive.com/host/0B1O5Onb__DjSRHJZeURzeW96dG8';
        // debug url
        // var url = 'https://www.googledrive.com/host/0B1O5Onb__DjSeldTZnFseHVBZGc';
        var options = window.localStorage.getItem("options");
        if (options !== null) {
            options = escape(options);
            console.log('Showing config with options=' + options);
            Pebble.openURL(url + '?options=' + options);
        } else {
            console.log('Showing config with no options');
            Pebble.openURL(url);
        }
    }
);

Pebble.addEventListener("webviewclosed",
    function(e) {
        console.log('Got response: ' + e.response);
        var config = JSON.parse(e.response);
        
        window.localStorage.options = JSON.stringify(config);
        Pebble.sendAppMessage(config,
                              function(e) {},
                              function(e) {
                                  console.log('Error sending message to watch: ' + JSON.stringify(e));
                              });
    }
);

 
