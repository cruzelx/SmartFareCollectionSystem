import { Mongoose, modelNames } from "mongoose";

var schema = new Mongoose.Schema({
    serialNumber: 'string',
    deduction: 'string',
    fromTo: 'string',
    remaining: 'string',
    timeOfEntry: {
        type: Date
    } 
});

