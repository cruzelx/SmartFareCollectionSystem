const express = require('express');
const app = express();
const bluetooth = require('node-bluetooth');
const device = new bluetooth.DeviceINQ();
const port = 3000;
var format = require('date-format');
var mongoose = require('mongoose');
var bodyParser = require('body-parser');
var listum = [];
var receivedData = [];
var receivedDataArray = [];
var urlencoded = bodyParser.urlencoded({ extended: true });

device.listPairedDevices(console.log);
//--------------Schemas------------------------------

var schema = new mongoose.Schema({
    serialNumber: String,
    mode: String,
    deduction: String,
    fromTo: String,
    remaining: String,
    rechargedAmount: String,
    timeOfEntry: {
        type: Date,
        default: Date.now()
    }

});

var regschema = new mongoose.Schema({
    serialNumber: String,
    username: String,
    password: String,
    timeOfEntry: {
        type: Date,
        default: Date.now()
    }
});

var Registration = mongoose.model('Registration', regschema);

var Transaction = mongoose.model('Transaction', schema);

//--------------------Database-----------------------

mongoose.connect('mongodb://localhost:27017/smartfarecollectionsystem', { useNewUrlParser: true });
var db = mongoose.connection;
db.on('error', console.error.bind(console, 'connection error'));
db.once('open', function () {
    console.log("connected to database--->");
});

//--------------------Bluetooth----------------------
device
    .on('finished', console.log.bind(console, 'finished'))
    .on('found', function found(address, name) {
        console.log('Found: ' + address + ' with name ' + name);
        if (name == "HC-06") {
            device.findSerialPortChannel(address, function (channel) {
                console.log('Found RFCOMM channel for serial port on %s: ', name, channel);
                bluetooth.connect(address, channel, function (err, connection) {
                    if (err) return console.error(err);
                    connection.delimiter = Buffer.from('\r\n', 'utf-8');

                    connection.write(new Buffer("Helloworldhowareyouababy"), () => {
                        console.log('wrote');
                    });
                    connection.on('data', (buffer) => {

                        console.log('received message: ', buffer.toString());

                        listum.push(buffer.toString());
                        receivedData = listum.join(' ').replace(/ /g, '');
                        console.log(receivedData);
                        receivedDataArray = receivedData.split("\r\n");
                        console.log(receivedDataArray);
                        for (let i = 0; i < receivedDataArray.length; i++) {
                            if (receivedDataArray[i] == '') {
                                Registration.countDocuments({ serialNumber: receivedDataArray[1] }, function (err, count) {
                                    if (err) console.log(err);
                                    else if(count>0) {
                                        if (receivedDataArray[0] == "payment") {
                                            Transaction({
                                                mode: receivedDataArray[0],
                                                serialNumber: receivedDataArray[1],
                                                deduction: receivedDataArray[2],
                                                fromTo: receivedDataArray[3],
                                                remaining: receivedDataArray[4],
                                                timeOfEntry: Date.now()
                                            }).save((err) => {
                                                console.log(err);
                                            });
                                        } else if (receivedDataArray[0] == "recharge") {
                                            Transaction({
                                                mode: receivedDataArray[0],
                                                serialNumber: receivedDataArray[1],
                                                rechargedAmount: receivedDataArray[2],
                                                remaining: receivedDataArray[3],
                                                timeOfEntry: Date.now()
                                            }).save((err) => {
                                                console.log(err);
                                            });
                                        }
                                    }
                                });


                                listum = [];
                            }


                        }
                        connection.write(new Buffer("ready"), () => {
                            console.log('wrote');
                        });
                    });
                });
            });
        } else return console.error("bluetooth device not connected");
    });



device.scan();
console.log("after");

app.use(bodyParser.json());
    app.post('/login',(req,res)=>{
        Registration.findOne({serialNumber: req.body.serialNum, password: req.body.password},(err,docs)=>{
            console.log(docs);
            if(err) res.send("Account Not found");
            else{
              console.log("found");
              res.send(docs);
            }
        });
    });

app.post('/registration',(req,res)=>{
console.log("registration accessed",req.body);
    Registration({
	serialNumber: req.body.serialNum,
	password: req.body.password,
	username: req.body.username,
	timeOfEntry: Date.now()
}).save((error)=>{console.log(error)});
});

app.post('/transactions',(req,res)=>{
  Transaction.find({serialNumber: req.body.serialNum},(err,docs)=>{
	if(!docs) res.send("No Transactions Found");
    	else if(err) res.send("Error Fetching Data");
	else res.send(docs);
    
  });
});

app.get('/', (req, res) => res.send('helloworld'));
app.listen(port, () => console.log(`listening on ${port}`));
