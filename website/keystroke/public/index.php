<!DOCTYPE html>
<html>
    <head>
        <title>Keystroke Dynamics Analysis</title>
        <!--{ Meta }-->
        <meta http-equiv="Content-Type" content="text/html;charset=utf-8">
        <!--{ CSS }-->
        <link href="/static/css/bootstrap.css" rel="stylesheet">
        <style type="text/css">
			body {
				padding-top: 20px;
				padding-bottom: 40px;
			}
			.sidebar-nav {
				padding: 18px 0;
			}
        </style>
        <link href="/static/css/bootstrap-responsive.min.css" rel="stylesheet">
    </head>
    <body>
        <div class="container">
            <div class="row">
            	<div><h1>Keystroke Dynamics Analysis</h1></div>
                <div id="fields" class="span12">
                    <!-- <textarea class="span6" rows="7"></textarea><br/> -->
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <input type="text"></input>
                    <br />
                    <button class="btn btn-small" onclick="javascript:printData()">Show Results</button>
                    <button class="btn btn-small" onclick="javascript:printJSON()">Show JSON</button>
                    <button class="btn btn-small" onclick="javascript:refreshData()">Reset</button>
                </div>
            </div>
            <div class="row">
                <div id="keystroke-report" class="span12"></div>
            </div>
        </div>
        <!--{ JavaScript (At the end for quicker page load.) }-->
        <script src="/static/js/jquery.min.js"></script>
        <script src="/static/js/bootstrap.min.js"></script>
        <script src="/static/js/keystroke.js"></script>
    </body>
</html>
