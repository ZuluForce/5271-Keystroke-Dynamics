/**
 * This is used to collect biometric keystroke data.
 * @author Brian T Maurer
 */

var last_keydown = {
	timeStamp: undefined,
	which: undefined,
};
var key_press_queue = {};
var press_times = {};
var fly_times = {};

var ids_to_keys = {
	/* letters */
	65:"a",	66:"b",	67:"c",	68:"d",	69:"e", 70:"f",	71:"g",	72:"h",	73:"i",	74:"j",
	75:"k",	76:"l",	77:"m",	78:"n",	79:"o",	80:"p",	81:"q",	82:"r",	83:"s",	84:"t",
	85:"u",	86:"v",	87:"w",	88:"x",	89:"y",	90:"z",
	/* other */
	16:"shift", 32:"spc",
	8:"del", 9:"tab", 13:"return", 17:"ctrl", 18:"opt", 20:"cps", 91:"cmd",
	/* numbers */
	48:"0", 49:"1", 50:"2", 51:"3",	52:"4",	53:"5",	54:"6",	55:"7",	56:"8",	57:"9",
	/* arrows */
	37:"left", 38:"right", 39:"up", 40:"down",
	/* symbols */
	186:":", 187:"=", 189:"-", 188:",", 190:".", 191:"/", 192:"`", 219:"[", 220:"\\",
	221:"]", 222:"'",
};

$(document).ready(function() {

	$(document).keyup(function(e) {
		if (key_press_queue[e.which] != undefined) {
			if (press_times[e.which] == undefined) {
				press_times[e.which] = [];
			}
			press_times[e.which].push(e.timeStamp-key_press_queue[e.which]);
			delete key_press_queue[e.which];
		}
	});

	$(document).keydown(function(e) {
		// 1st, deal with pres times:
		if (ids_to_keys[last_keydown.which] != undefined) {
			key_press_queue[e.which] = e.timeStamp;
		}

		// 2nd, Deal with flytimes:
		// first keypress on the page
		if (last_keydown.timeStamp === undefined) {
			last_keydown.timeStamp = e.timeStamp;
			last_keydown.which = e.which;
		}
		// all other keypresses
		else {
			// get flytime
			flytime = e.timeStamp - last_keydown.timeStamp;
			// add flytime data (for keys we want to track)
			if (ids_to_keys[last_keydown.which] != undefined && ids_to_keys[e.which] != undefined ) {
				if (fly_times[last_keydown.which] === undefined) {
					fly_times[last_keydown.which] = {};
				}
				if (fly_times[last_keydown.which][e.which] === undefined) {
					fly_times[last_keydown.which][e.which] = [];
				}
				fly_times[last_keydown.which][e.which].push(flytime);
			}
			// update last_keydown
			last_keydown.timeStamp = e.timeStamp;
			last_keydown.which = e.which;
		}
	});

	printData = function() {
		$("#keystroke-report").append("<h3>Fly Times</h3>");
		for (k1 in fly_times) {
			for (k2 in fly_times[k1]) {
				$("#keystroke-report").append("<strong>"+ids_to_keys[k1]+"</strong>&rarr;"+ids_to_keys[k2]+" ["+fly_times[k1][k2]+"]<br/>");
			}
			$("#keystroke-report").append("<br/>");
		}
		$("#keystroke-report").append("<h3>Keypress Times</h3>");
		for (k in press_times) {
			$("#keystroke-report").append("<strong>"+ids_to_keys[k]+"</strong> ["+press_times[k]+"]<br/>");
		}
	};
});
