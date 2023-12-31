import 'package:flutter/material.dart';
import 'package:flutter/services.dart'; // for TextInputFormatters
import 'package:grocery_app/common_widgets/app_button.dart';
import 'package:grocery_app/common_widgets/app_text.dart';
import 'package:grocery_app/screens/dashboard/dashboard_screen.dart';
import 'package:grocery_app/styles/colors.dart';
import 'package:flutter_svg/flutter_svg.dart';

class WelcomeScreen extends StatelessWidget {
  final TextEditingController ipAddressController =
  TextEditingController(text: "192.168.31.25");
  final TextEditingController portController = TextEditingController(text: "8080");

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Color(0xFF1C1C1C), // Updated background color
      body: Container(
        padding: EdgeInsets.symmetric(horizontal: 30),
        child: Center(
          child: Column(
            mainAxisSize: MainAxisSize.max,
            children: [
              SizedBox(height: 180),
              icon(),
              SizedBox(height: 40),
              textField("Enter IP Address", ipAddressController, isIPAddress: true),
              SizedBox(height: 20),
              textField("Enter Port", portController),
              SizedBox(height: 40),
              connectButton(context),
              SizedBox(height: 40),
            ],
          ),
        ),
      ),
    );
  }

  Widget icon() {
    String iconPath = "assets/images/roboticarm.svg";
    return SvgPicture.asset(
      iconPath,
      width: 48,
      height: 56,
    );
  }

  Widget welcomeTextWidget() {
    return Column(
      children: [
        AppText(
          text: "Welcome",
          fontSize: 31,
          fontWeight: FontWeight.w600,
          color: Colors.white,
        ),
        AppText(
          text: "to our SCARA robo",
          fontSize: 31,
          fontWeight: FontWeight.w600,
          color: Colors.white,
        ),
      ],
    );
  }

  Widget sloganText() {
    return AppText(
      text: "The Best SCARA Artist You Ever Seen",
      fontSize: 16,
      fontWeight: FontWeight.w600,
      color: Color(0xffFCFCFC).withOpacity(0.7),
    );
  }

  Widget textField(String labelText, TextEditingController controller, {bool isIPAddress = false}) {
    TextInputType inputType = isIPAddress
        ? TextInputType.numberWithOptions(decimal: true)
        : TextInputType.number;

    List<TextInputFormatter> inputFormatters = isIPAddress
        ? [
      FilteringTextInputFormatter.allow(RegExp(r'[0-9.]')),
    ]
        : [
      FilteringTextInputFormatter.digitsOnly,
    ];

    return TextFormField(
      controller: controller,
      keyboardType: inputType,
      inputFormatters: inputFormatters,
      decoration: InputDecoration(
        labelText: labelText,
        labelStyle: TextStyle(color: Color(0xFF33C2FF)),
        enabledBorder: OutlineInputBorder(
          borderSide: BorderSide(color: Color(0xFF33C2FF)),
        ),
        focusedBorder: OutlineInputBorder(
          borderSide: BorderSide(color: Color(0xFF33C2FF)),
        ),
      ),
      style: TextStyle(color: Colors.white),
    );
  }

  Widget connectButton(BuildContext context) {
    return ElevatedButton(
      style: ElevatedButton.styleFrom(
        primary: Color(0xFF33C2FF), // Background color
        onPrimary: Color(0xFF424242), // Text color
        side: BorderSide(width: 1, color: Color(0xFF767676)), // Border
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(20), // Adjust the radius as needed
        ),
      ),
      onPressed: () {
        onGetStartedClicked(context);
      },
      child: Container(
        padding: EdgeInsets.symmetric(vertical: 25),
        child: Text(
          "Connect to Device!",
          style: TextStyle(
            fontWeight: FontWeight.w600,
            fontSize: 16,
          ),
        ),
      ),
    );
  }

  void onGetStartedClicked(BuildContext context) {
    String ipAddress = ipAddressController.text;
    int port = int.tryParse(portController.text) ?? 8080; // Use default port 8080 if parsing fails

    Navigator.of(context).pushReplacement(
      MaterialPageRoute(
        builder: (BuildContext context) {
          return DashboardScreen(ipAddress: ipAddress, port: port);
        },
      ),
    );
  }
}
