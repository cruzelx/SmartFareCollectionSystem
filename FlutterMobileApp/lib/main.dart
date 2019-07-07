import 'package:flutter/material.dart';
import 'Login_Page.dart';
import 'signup.dart';

void main()=>runApp(FareColSys());

class FareColSys extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: MainBody(),
      debugShowCheckedModeBanner: false,
      theme: new ThemeData(primaryColor: Colors.pinkAccent),

    );
  }
}

class MainBody extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      resizeToAvoidBottomPadding: false,
      body:Login_Page(),
     //body:signUp(),
    );
  }
}


