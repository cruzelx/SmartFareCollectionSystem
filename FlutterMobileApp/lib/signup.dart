import 'package:flutter/material.dart';
import 'dart:async';
import 'dart:convert';
import 'dart:io';
import 'package:http/http.dart' as http;

void _formSendReg(username, password, serialNumber) {
  final regurl = "http://192.168.0.101:3000/registration";

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
              'username': username
            }))
        .then((response) {
      print("${response.body}");
    });
  }

  _sendForm();
}

final _formKey = GlobalKey<FormState>();

class signUp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: signup_form(),
    );
  }
}

class signup_form extends StatefulWidget {
  @override
  _signup_formState createState() => _signup_formState();
}

class _signup_formState extends State<signup_form> {
  final serialController = TextEditingController();
  final userController = TextEditingController();
  final passwordController = TextEditingController();

  @override
  void dispose() {
    serialController.dispose();
    super.dispose();
    userController.dispose();
    super.dispose();
    passwordController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final headerIcon = Icon(
      Icons.account_circle,
      color: Colors.pinkAccent,
      size: 80.0,
    );

    final serialNum = Theme(
        child: TextField(
          controller: serialController,
          keyboardType: TextInputType.number,
          decoration: new InputDecoration(
            prefixIcon: Icon(Icons.keyboard),
            labelText: "Serial Number",
            border: OutlineInputBorder(),
          ),
        ),
        data: Theme.of(context).copyWith(primaryColor: Colors.pinkAccent));

    final passWord = Theme(
        child: TextField(
          obscureText: true,
          controller: passwordController,
          decoration: new InputDecoration(
            prefixIcon: Icon(Icons.lock),
            labelText: "Password",
            border: OutlineInputBorder(),
          ),
        ),
        data: Theme.of(context).copyWith(primaryColor: Colors.pinkAccent));

    final userName = Theme(
        child: TextField(
          controller: userController,
          autofocus: true,
          decoration: new InputDecoration(
            prefixIcon: Icon(Icons.supervised_user_circle),
            labelText: "Username",
            border: OutlineInputBorder(),
          ),
        ),
        data: Theme.of(context).copyWith(primaryColor: Colors.pinkAccent));

    final signUpBotton = Expanded(
      child: GestureDetector(
        onTap: () {
          _formSendReg(userController.text, passwordController.text,
              serialController.text);
            Navigator.pop(context);
        },
        child: new Container(
          height: 50.0,
          width: double.infinity,
          alignment: Alignment.center,
          decoration: new BoxDecoration(
            color: Colors.pinkAccent,
            borderRadius: new BorderRadius.circular(10.0),
          ),
          child: Text("Sign Up",
              style: TextStyle(color: Colors.white, fontSize: 16.0)),
        ),
      ),
    );

    return Scaffold(
      resizeToAvoidBottomPadding: false,
      body: Center(
        child: Padding(
          padding: const EdgeInsets.symmetric(horizontal: 12.0),
          child: Form(
            key: _formKey,
            child: new Column(
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.center,
              children: <Widget>[
                headerIcon,
                SizedBox(
                  height: 10.0,
                ),
                userName,
                SizedBox(
                  height: 10.0,
                ),
                passWord,
                SizedBox(
                  height: 10.0,
                ),
                serialNum,
                SizedBox(
                  height: 20.0,
                ),
                Row(
                  children: <Widget>[
                    signUpBotton,
                  ],
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
