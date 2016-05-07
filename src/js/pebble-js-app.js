var JSAPPSTATE = Object.freeze({INITWAIT:1, READY:2, INLOOP:3, REFRESHWAIT: 4});
var APPMSG_TYPE = Object.freeze({TYPE_ADD:0, TYPE_CANCEL:1, TYPE_LOAD:2, TYPE_STATUS:3});
var jsAppState = JSAPPSTATE.INITWAIT;
var TIMEOUT = 5000;
var bdata;
var bdataidx;
var aud = new Intl.NumberFormat("en-AU", { style: "decimal", minimumFractionDigits: 0, maximumFractionDigits: 0 });
var req;
var timer;

var GAS_APP = 'https://script.google.com/macros/s/AKfycby5JM15iJ5NuBjswu9swwB7Vpx666aCXObA2NHO0-eFnchhjTQ/exec';

function sendStatusMessage(msg) {
    console.log('sendStatusmessage ' + msg);
    Pebble.sendAppMessage( {
	'MSGKEY_TYPE': APPMSG_TYPE.TYPE_STATUS,
	'MSGKEY_STATUSTEXT': msg
    });
}

function addRequestOnload() {
    if (this.readyState == 4 && this.status == 200) {
	console.log('addreq ok');
    } else {
	console.log('addreq fail ' + this.readyState + ", " + this.status);
    }
}

function addExpense(description, category, value) {
    console.log('add expense');
    
    req = new XMLHttpRequest();
    reqbody = "desc=" + description + "&value=" + value;
    req.open('POST', GAS_APP);   
    req.timeout = TIMEOUT;
    req.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    req.addEventListener('load', addRequestOnload);
    req.send(reqbody);
}

function budget_data_loaded() {
    if (this.readyState == 4 && this.status == 200) {  
	clearTimeout(timer);
	bdata = JSON.parse(this.responseText);
	bdataidx = 0;
	sendOneItem();
	jsAppState = JSAPPSTATE.INLOOP;
    }
}

function request_error() {
    req.abort();
    sendStatusMessage('request error ' + this.status);
}

function request_timeout() {
    sendStatusMessage("request timeout");
    req.abort();
}

function getBudgetInfo() {   
    
    req = new XMLHttpRequest();
    req.open('GET', GAS_APP);   
    req.addEventListener('load', budget_data_loaded);
    req.addEventListener('error', request_error);

    // timeout on XHR doesn't work so user a separate timer instead
    sendStatusMessage('sending request...');
    timer = setTimeout(request_timeout, TIMEOUT);
    req.send();
    sendStatusMessage('sent request...');
}

function sendOneItem() {
  Pebble.sendAppMessage( 
      { 'MSGKEY_TYPE': APPMSG_TYPE.TYPE_ADD, 
	'MSGKEY_NAME': bdata[bdataidx][0],
	'MSGKEY_AMOUNT': aud.format(bdata[bdataidx][1]) },
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

function handleAppMessage(e) {
   console.log("appmessage received");

    var dict = e.payload;
    if (!('MSGKEY_TYPE' in dict)) {
	console.log('Received appmessage with no type');
    } else {
	switch (dict['MSGKEY_TYPE']) {
	case APPMSG_TYPE.TYPE_ADD:
	    if ('MSGKEY_NEWEXPENSEDESC' in dict && 'MSGKEY_NEWEXPENSEVALUE' in dict) {
		addExpense(dict['MSGKEY_NEWEXPENSEDESC'], '', dict['MSGKEY_NEWEXPENSEVALUE']);
	    } else {
		console.log('Received add appmessage without description & value');
	    }
	    break;

	case APPMSG_TYPE.TYPE_CANCEL:
	    break;

	case APPMSG_TYPE.TYPE_LOAD:
	    break;

	case APPMSG_TYPE.TYPE_STATUS:
	    break;

	default:
	    console.log('Received appmessage with unknown type' + dict['MSGKEY_TYPE']);
	    break;
	}
    }
 }
    
Pebble.addEventListener('appmessage', handleAppMessage);
