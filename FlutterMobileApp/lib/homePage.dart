import 'package:flutter/material.dart';
import 'dart:async';
import 'dart:convert';
import 'dart:io';
import 'package:http/http.dart' as http;
import 'package:smart_fare_collection_system/Login_Page.dart';

class Transaction{
  final String mode;
  final String serialNumber;
  final String deduction;
  final String fromTo;
  final String remaining;
  final String timeOfEntry;
  final String rechargedAmount;

  Transaction(this.mode, this.serialNumber,this.deduction,this.fromTo,this.remaining,this.timeOfEntry, this.rechargedAmount);

}

class homePage extends StatefulWidget {
   final UserData data;
  homePage({this.data});

  @override
  _homePageState createState() => _homePageState();
}

class _homePageState extends State<homePage> {
  
  Future<List<Transaction>> _getTransactions() async{
    var response = await http.post(
      'http://192.168.0.101:3000/transactions',
      headers:  {
              "Accept":"application/json",
              "Content-Type":"application/json"
            },
      body: json.encode({
        'serialNum': widget.data.serialNumber
      })
    
    );

      var jsonData = json.decode(response.body);
      print(response.body);
      print("----------------------------------------");
      print(jsonData);
      print("----------------------------------------");

      List<Transaction> transactions = [];
      for (var t in jsonData){
        Transaction transact = Transaction(t["mode"],t["serialNumber"],t["deduction"],t["fromTo"],t["remaining"],t["timeOfEntry"],t["rechargedAmount"]);
        transactions.add(transact);
      }
      print(transactions.length);
      return transactions;
      
    }
    
  
  

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Transactions", style: TextStyle(fontSize: 20.0),textAlign: TextAlign.center),
        actions: <Widget>[
          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 10.0),
            child: Container(
              alignment: Alignment.center,
             child: Text('SN: ${widget.data.serialNumber}',style: TextStyle(fontSize: 18.0), textAlign: TextAlign.center,),
            ),
          ),
        ],
      ),
      body: Center(
        child: Padding(
          padding: EdgeInsets.symmetric(horizontal: 10.0),
          child: FutureBuilder(
            future: _getTransactions(),
            builder: (BuildContext context, AsyncSnapshot snapshot ){
              if(snapshot.data == null){
                return Container(
                  child: Center(
                    child: Text("Loading"),
                  ),
                );
              }else{
              return ListView.builder(
                itemCount: snapshot.data.length,
                itemBuilder: (BuildContext context,int index){
                  return Card(
                    child: Container(
                      decoration: new BoxDecoration(
                        color: Colors.pinkAccent,
                        borderRadius: new BorderRadius.circular(10.0),
                        boxShadow: [
                          BoxShadow(
                            color: Colors.grey,
                            blurRadius: 5.0,
                          ),
                        ],
                        
                      ),
                      child: Center(
                        child: Column(
                          children: <Widget>[
                            Padding(
                              padding: const EdgeInsets.all(8.0),
                              child: Row(
                                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                                children: <Widget>[
                                  Text('${snapshot.data[index].timeOfEntry}',style: TextStyle(color: Colors.white)),
                                  Text('Mode: ${snapshot.data[index].mode}',style: TextStyle(color: Colors.white,fontWeight: FontWeight.bold))
                                ],
                              ),
                            ),
                            Divider(color: Colors.white),
                            (snapshot.data[index].mode == "recharge") ?     
                              Padding(
                              padding: EdgeInsets.all(8.0),
                              child: Row(
                                
                                children: <Widget>[
                                 Column(
                                   crossAxisAlignment: CrossAxisAlignment.start,
                                   children: <Widget>[
                                  Text('Recharged Amount: ${snapshot.data[index].rechargedAmount}', style: TextStyle(color: Colors.white),),
                                  SizedBox(height: 3.0),
                                  Text('Remaining Amount: ${snapshot.data[index].remaining}',style: TextStyle(color: Colors.white)),
                                  SizedBox(height: 3.0),
                                   ],
                                 ),
                                ],
                              )
                            ): Padding(
                              padding: EdgeInsets.all(8.0),
                              child: Row(
                                children: <Widget>[
                                 Column(
                                   crossAxisAlignment: CrossAxisAlignment.start,
                                   children: <Widget>[
                                  Text('Recharged Amount: ${snapshot.data[index].deduction}', style: TextStyle(color: Colors.white),),
                                  SizedBox(height: 3.0),
                                  Text('Remaining Amount: ${snapshot.data[index].remaining}',style: TextStyle(color: Colors.white)),
                                  SizedBox(height: 3.0),
                                  Text('Remaining Amount: ${snapshot.data[index].fromTo}',style: TextStyle(color: Colors.white)),
                                   SizedBox(height: 3.0),
                                   ],
                                 ),
                                ],
                              ),
                            )
                          ],
                        ),
                      ),
                    ),
                    );
                },
                  );
              }
            },
                
              ),
            
        ),
          ),
        );
      
    
  }
}