var http = require ('http');
var url = require('url');
var mysql = require('mysql');

const hostname = 'team-homer.glynny.org';
const port = 3000;

function callback (user, pass, result, response)
{
	console.log("Result: '" + JSON.stringify(result)+"'");

	var text= '<body>';

	if (user) {
		if (pass) {
			if (result.length == 0)
				text += 'No such user '+user;
			else if (result[0].pass != pass)
				text += 'Wrong password for '+user;
			else
				text += 'User '+user+' logged in with password '+pass;
		}
		else
			text += 'No password given for '+user;
	}
	else
		text += 'No user given';

	text += '</body>';
	response.writeHead (200, {'Content-Type': 'text/html'});
        response.end(text);
}

function login (user, pass, req, res)
{
	var con = mysql.createConnection({
		host: "127.0.0.1",
		user: "homer",
		password: "bart",
		database: "homerdb"
	});

	var sql="select * from users where name='"+user+"';";
	con.connect(function(err) {
		if (err) throw err; 
		console.log("Connected query="+sql);
		con.query(sql, function (err, result, fields) {
			if (err) throw err;
			callback (user, pass, result, res);
		});
	});
}

const server=http.createServer((req, res) => {
	var q = url.parse(req.url, true);
	var s = q.pathname.split('/');

	switch (s[1])
	{
	case "login":
		login (s[2], s[3], req, res);
		break;
	default:
		res.statusCode = 200;
		res.setHeader ("Content-Type", "text/plain");
		res.end ("Hello world\n");
		break;
	}
});

server.listen(port,hostname, () => {
	console.log('homer server running at http://'+hostname+':'+port);
});;
