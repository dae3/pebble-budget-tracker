var JSAPPSTATE = Object.freeze({INITWAIT:1, READY:2, INLOOP:3, REFRESHWAIT: 4});
var jsAppState = JSAPPSTATE.INITWAIT;
var TIMEOUT = 2000;
var bdata;
var bdataidx;
var aud = new Intl.NumberFormat("en-AU", { style: "currency", currency: "AUD", minimumFractionDigits: 0, maximumFractionDigits: 0 });

	
function getBudgetInfo() {   
  var req = new XMLHttpRequest();
  req.open('GET', 'https://script.google.com/macros/s/AKfycby5JM15iJ5NuBjswu9swwB7Vpx666aCXObA2NHO0-eFnchhjTQ/exec');   
  req.timeout = TIMEOUT;
  req.onload = function(e) {
    if (req.readyState == 4 && req.status == 200) {  
      bdata = JSON.parse(req.responseText);
      bdataidx = 0;
      sendOneItem();
      jsAppState = JSAPPSTATE.INLOOP;
    }
  };
  req.ontimeout = function(e) {
    console.log("getBudgetInfo timeout after " + TIMEOUT + " seconds");
    jsAppState = JSAPPSTATE.READY;
    Pebble.sendAppMessage( { 'MSGKEY_TIMEOUT': TIMEOUT });  
  };
  console.log("sending XHR to GAS application");
  req.send();
}

function sendOneItem() {
  Pebble.sendAppMessage( 
    { 'MSGKEY_NAME': bdata[bdataidx][0], 'MSGKEY_AMOUNT': aud.format(bdata[bdataidx][1]) },
    function(e) { if (++bdataidx == bdata.length) { 
      jsAppState = JSAPPSTATE.READY;
    } else {
      sendOneItem(); 
    }},
    function(e) { 
      // assume it's a not ready error
      // TODO either determine the actual error, or implement a retry limit
      console.log("sendAppMessage failed %s", e);
      jsAppState = JSAPPSTATE.INLOOP;
    }
  );
}      

function readyMessageHandler(e) {
  switch(jsAppState) {
    case JSAPPSTATE.INITWAIT:
      jsAppState = JSAPPSTATE.READY;
    case JSAPPSTATE.READY:  
        console.log("readymsg INITWAIT");        
        getBudgetInfo();
      break;      
    
    case JSAPPSTATE.INLOOP:
      console.log("readymsg INLOOP");
      sendOneItem();
      break;
      
    default:
      console.log("ready message handler default jsappstate: " + jsAppState.toString());
      break;
  }
}

Pebble.addEventListener('ready', readyMessageHandler);

//Pebble.addEventListener('appmessage', function(e) { console.log("appmsg"); getBudgetInfo(); });
Pebble.addEventListener('appmessage', function(e) {
	console.log("appmsg");
	addExpense();
	});
