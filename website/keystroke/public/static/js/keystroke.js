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


	// Extend array capabilities:
	Array.prototype.sum = function() {
		var sum = 0;
		for (var i=0; i<this.length; i++) {
			sum += this[i];
		}
		return sum;
	};

	Array.prototype.mean = function() {
		var sum = this.sum();
		return sum/this.length;
	};

	Array.prototype.variance = function() {
		var mean = this.mean();
		var sum_of_square_differences = 0;
		for (var i=0; i<this.length; i++) {
			sum_of_square_differences = sum_of_square_differences + (mean-this[i]) * (mean-this[i]);
		}
		if (this.length-1 > 0) {
			return sum_of_square_differences/(this.length-1);
		}
		else {
			return 0;
		}
	};

	Array.prototype.standardDeviation = function() {
		return Math.sqrt(this.variance());
	};
	Array.prototype.stdev = Array.prototype.standardDeviation;

	refreshData = function() {
		press_times = {};
		fly_times = {};
		$("#fields > input").val("");
		$("#keystroke-report").html("");
	};

	printJSON = function() {
		$("#keystroke-report").html("");
		var text = "<h3>Fly Times</h3>";
		text += "<p>"+JSON.stringify(fly_times)+"</p>";
		text += "<h3>Press Times</h3>";
		text += "<p>"+JSON.stringify(press_times)+"</p>";

		$("#keystroke-report").append(text);
	};

	printData = function() {
		$("#keystroke-report").html("");
		var text = "<h3>Fly Times</h3>";
		text += "<dl class=\"dl-horizontal\">";
		for (k1 in fly_times) {
			for (k2 in fly_times[k1]) {
				text += "<dt>"+ids_to_keys[k1]+"&rarr;"+ids_to_keys[k2]+"</dt> <dd>(&mu;, &sigma;): ("+fly_times[k1][k2].mean().toFixed(0)+", "+fly_times[k1][k2].stdev().toFixed(2)+/*"<br/><strong>[</strong>"+fly_times[k1][k2]+"<strong>]</strong>*/")</dd>";
			}
		}
		text += "</dl>";
		text += "<h3>Press Times</h3>";
		text += "<dl class=\"dl-horizontal\">";
		for (k in press_times) {
			text += "<dt>"+ids_to_keys[k]+"</dt> <dd>(&mu;, &sigma;): ("+press_times[k].mean().toFixed(0)+", "+press_times[k].stdev().toFixed(2)+/*"</strong> ["+press_times[k]+"]*/")</dd>";
		}
		text += "</dl>";
		$("#keystroke-report").append(text);
	};
});

















