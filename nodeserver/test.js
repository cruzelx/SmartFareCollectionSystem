const express = require('express');
const app = express();
const bluetooth = require('node-bluetooth');
const device = new bluetooth.DeviceINQ();
const port = 3000;
var mongoose = require('mongoose');
var bodyParser = require('body-parser');



app.get('/', (req, res) => res.send('helloworld'));
app.listen(port, () => console.log(`listening on ${port}`));