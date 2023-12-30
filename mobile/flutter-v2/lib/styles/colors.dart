import 'dart:ui';

import 'package:flutter/material.dart';

class AppColors {
  //One instance, needs factory
  static AppColors? _instance;
  factory AppColors() => _instance ??= AppColors._();

  AppColors._();

  static const primaryColor = Color.fromARGB(255, 83, 92, 177);
  static const darkGrey = Color(0xff7C7C7C);
}
