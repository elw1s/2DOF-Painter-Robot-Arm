import 'package:flutter/material.dart';
import 'package:flutter/services.dart'; // for TextInputFormatters
import 'package:grocery_app/common_widgets/app_button.dart';
import 'package:grocery_app/common_widgets/app_text.dart';
import 'package:grocery_app/screens/dashboard/dashboard_screen.dart';
import 'package:grocery_app/styles/colors.dart';
import 'package:flutter_svg/flutter_svg.dart';

class WelcomeScreen extends StatelessWidget {
  final String imagePath = "assets/images/welcome_image.jpg";
  final TextEditingController ipAddressController =
  TextEditingController(text: "127.0.0.1");
  final TextEditingController portController = TextEditingController(text: "8080");

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Color(0xFF1C1C1C), // Updated background color
      body: Container(
        padding: EdgeInsets.symmetric(horizontal: 30),
        decoration: BoxDecoration(
          image: DecorationImage(
            image: AssetImage(imagePath),
            fit: BoxFit.cover,
          ),
        ),
        child: Center(
          child: Column(
            mainAxisSize: MainAxisSize.max,
            children: [
              Spacer(),
              icon(),
              SizedBox(
                height: 20,
              ),
              welcomeTextWidget(),
              SizedBox(
                height: 10,
              ),
              sloganText(),
              SizedBox(
                height: 40,
              ),
              textField("Enter IP Address", ipAddressController),
              SizedBox(
                height: 20,
              ),
              textField("Enter Port", portController),
              SizedBox(
                height: 40,
              ),
              connectButton(context),
              SizedBox(
                height: 40,
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget icon() {
    String iconPath = "assets/icons/app_icon.svg";
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

  Widget textField(String labelText, TextEditingController controller) {
    return TextFormField(
      controller: controller,
      keyboardType: TextInputType.number,
      inputFormatters: <TextInputFormatter>[
        FilteringTextInputFormatter.digitsOnly
      ],
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
    return AppButton(
      label: "Connect to Device!",
      fontWeight: FontWeight.w600,
      padding: EdgeInsets.symmetric(vertical: 25),
      onPressed: () {
        onGetStartedClicked(context);
      },
    );
  }

  void onGetStartedClicked(BuildContext context) {
    Navigator.of(context).pushReplacement(
      MaterialPageRoute(
        builder: (BuildContext context) {
          return DashboardScreen();
        },
      ),
    );
  }
}
