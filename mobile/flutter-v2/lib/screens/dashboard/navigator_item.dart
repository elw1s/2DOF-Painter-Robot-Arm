import 'package:flutter/material.dart';
import 'package:grocery_app/screens/XOXScreen.dart';

import 'package:grocery_app/screens/home/home_screen.dart';

import '../favourite_screen.dart';

class NavigatorItem {
  final String label;
  final String iconPath;
  final int index;
  final Widget screen;

  NavigatorItem(this.label, this.iconPath, this.index, this.screen);
}

List<NavigatorItem> navigatorItems = [
  NavigatorItem("Image", "assets/icons/shop_icon.svg", 0, HomeScreen()),
  /*NavigatorItem(
      "Game", "assets/icons/favourite_icon.svg", 3, FavouriteScreen()),*/
  NavigatorItem(
      "Game", "assets/icons/favourite_icon.svg", 3, XOXScreen()),
];

