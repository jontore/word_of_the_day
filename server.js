var express    = require("express");
var mysql      = require('mysql');
var connection = mysql.createConnection({
  host     : 'localhost',
  user     : 'root',
  password : '',
  database : 'word_of_the_day'
});
var app = express();

connection.connect(function(err){
  if(!err) {
      console.log("Database is connected ... \n\n");
  } else {
      console.log("Error connecting database ... \n\n");
  }
});

app.get("/",function(req,res) {
  var randomId = parseInt(Math.random() * 1000000);
  connection.query('select * from dict where id = ' + randomId + ';', function(err, rows, fields) {
    if (!err) {
      res.send(rows[0].de + ' - ' + rows[0].en);
    } else {
      console.log('Error while performing Query.');
    }
  });
});

app.listen(3000);

