import 'package:flutter/material.dart';
import 'signup.dart';
import 'homePage.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';

class UserData{
  final String serialNumber;
  final String password;
  UserData({this.serialNumber,this.password});
  }

void _formSendLogin(password, serialNumber) {
  final regurl = "http://192.168.0.101:3000/login";

  _sendForm() async {
    await http
        .post(regurl,
            headers: {
              "Accept":"application/json",
              "Content-Type":"application/json"
            },
            body: json.encode({
              'serialNum': serialNumber,
              'password': password,
              
            }))
        .then((response) {
      print("${response.body}");
     
    });
  }

  _sendForm();
}

class Login_Page extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Login_Form(),
      resizeToAvoidBottomPadding: false,
      appBar: new AppBar(
        backgroundColor: Colors.transparent,
        elevation: 0.0,
        iconTheme: new IconThemeData(color: Colors.pinkAccent),
      ),
    );
  }
}

class Login_Form extends StatefulWidget {
  @override
  _Login_FormState createState() => _Login_FormState();
}

class _Login_FormState extends State<Login_Form> {

  final serialController = TextEditingController();
  final passwordController = TextEditingController();

  @override
  void dispose() {
    serialController.dispose();
    super.dispose();
    passwordController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final loginIcon = Icon(
      Icons.account_circle,
      color: Colors.pinkAccent,
      size: 80.0,
    );
    final serialNumField = Theme(
        child: TextField(
          controller: serialController,
          autofocus: true,
          keyboardType: TextInputType.number,
          decoration: new InputDecoration(
            prefixIcon: Icon(Icons.keyboard),
            labelText: 'Serial Number',
            border: OutlineInputBorder(),
          ),
        ),
        data: Theme.of(context).copyWith(primaryColor: Colors.pinkAccent));

    final passwordfield = Theme(
        child: TextField(
          controller: passwordController,
          autofocus: false,
          obscureText: true,
          decoration: new InputDecoration(
            prefixIcon: Icon(Icons.lock),
            labelText: 'Password',
            border: OutlineInputBorder(),
          ),
        ),
        data: Theme.of(context).copyWith(primaryColor: Colors.pinkAccent));

    final loginButton = Expanded(
      child: GestureDetector(
        onTap: () {
          _formSendLogin(passwordController.text,serialController.text);
          final userData = UserData(serialNumber: serialController.text, password: passwordController.text);
          Navigator.push(context,MaterialPageRoute(builder: (context)=>homePage(
            data: userData,
          )));
        },
        child: new Container(
          alignment: Alignment.center,
          height: 50.0,
          width: 75.0,
          decoration: new BoxDecoration(
            borderRadius: new BorderRadius.circular(10.0),
            color: Colors.pinkAccent,
          ),
          child: Text(
            "Login",
            style: TextStyle(color: Colors.white, fontSize: 18.0),
          ),
        ),
      ),
    );

    final signUpButton = Expanded(
      child: GestureDetector(
        onTap: () {
          
        },
        child: new Container(
          alignment: Alignment.center,
          height: 60.0,
          width: 75.0,
          child: Text(
            "Forgot Password?",
            style: TextStyle(color: Colors.blueAccent, fontSize: 16.0),
          ),
        ),
      ),
    );

    return Scaffold(
      resizeToAvoidBottomPadding: false,
      body: Center(
        child: Container(
          // decoration: BoxDecoration(
          //   gradient: LinearGradient(
          //     begin: Alignment.topCenter,
          //     end: Alignment.bottomCenter,
          //     colors: [const Color(0xff79030b), const Color(0xffee2236)],
          //   ),
          // ),
          child: Padding(
            padding: const EdgeInsets.all(12.0),
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.center,
              mainAxisAlignment: MainAxisAlignment.start,
              children: <Widget>[
                loginIcon,
                SizedBox(
                  height: 10.0,
                ),
                serialNumField,
                SizedBox(height: 10.0),
                passwordfield,
                SizedBox(
                  height: 20.0,
                ),
                Row(
                  children: <Widget>[
                    loginButton,
                    signUpButton,
                  ],
                ),
              ],
            ),
          ),
        ),
      ),
      bottomNavigationBar: new Container(
        height: 40.0,
        child: GestureDetector(
          onTap: () {
            Navigator.push(context, MaterialPageRoute(builder: (context)=>signup_form()));
          },
          child: new Column(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.center,
            children: <Widget>[
              Text("Create A New Account",
                  style: TextStyle(color: Colors.blueAccent, fontSize: 16.0)),
            ],
          ),
        ),
      ),
    );
  }
}
