var express = require('express');
var mysql = require('mysql');
var app = express();
var Connection = mysql.createConnection({

    host: 'localhost',
    user: 'root',
    password: '',
    database: 'organic farming'
});

Connection.connect(function (error) {
    if (!!error) {
        console.log('Error');
    } else {
        console.log('Connected');
    }
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

})

app.listen(1337);