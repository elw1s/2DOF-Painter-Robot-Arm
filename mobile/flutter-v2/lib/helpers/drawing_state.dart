import 'package:flutter/foundation.dart';

class DrawingState extends ChangeNotifier {
  bool isDrawing = false;
  bool draw = false;

  void setDrawing(bool value) {
    isDrawing = value;
    notifyListeners();
  }

  void setDraw(bool value) {
    draw = value;
    notifyListeners();
  }
}