import 'package:flutter/foundation.dart';

class DrawingState extends ChangeNotifier {
  bool isDrawing = false;
  bool draw = false;
  int totalLineNumber = 100;
  int drawnLineNumber = 0;

  void setDrawing(bool value) {
    isDrawing = value;
    notifyListeners();
  }

  void setDraw(bool value) {
    draw = value;
    notifyListeners();
  }

  void setTotalLineNumber(int value) {
    totalLineNumber = value;
    notifyListeners();
  }

  void setDrawnLineNumber(int value) {
    drawnLineNumber = value;
    notifyListeners();
  }
}