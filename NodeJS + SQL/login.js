var express = require('express');
var mysql = require('mysql');
var sessions = require('express-session');
var bodyParser = require('body-parser');
var app = express();

var Connection = mysql.createConnection({

    host: 'localhost',
    user: 'root',
    password: '',
    database: 'organic farming'
});

var herr = function(error) {
  if (!!error) {
    console.log("Error");
  } else {
    console.log("Connected");
  }
};
Connection.connect(herr);

app.get('/', function (req, resp) {
    resp.sendFile('index.html', {
        root: __dirname
    });
});

app.get('/', function (req, resp) {
    Connection.query("SELECT * FROM admin", function (error, rows, fields) {

        if (!!error) {
            console.log('Error in query');
        } else {
            console.log('Success  in query');
            console.log(rows);
        }
    });
    app.post('/', function (req, resp) {

        resp.end(JSON.stringify(req.body));
        if (req.body.username == 'admin' && req.body.password == 'admin') {
            session.id = req.body.username;
        }

    })

})

app.listen(1337);