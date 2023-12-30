import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter/rendering.dart';
import 'package:grocery_app/common_widgets/app_text.dart';
import 'dart:math';
import 'dart:typed_data';
import 'dart:ui' as ui;
import 'package:image/image.dart' as img;
import 'package:path_provider/path_provider.dart';
import 'package:image_gallery_saver/image_gallery_saver.dart';

class XOXGame extends StatefulWidget {
  @override
  _XOXGameState createState() => _XOXGameState();
}

class _XOXGameState extends State<XOXGame> {
  List<List<String>> board = List.generate(3, (i) => List<String>.filled(3, ''));
  bool isPlayerX = true;
  GlobalKey _boardKey = GlobalKey();
  List<List<int>> packet = [];


  @override
  void initState() {
    super.initState();
    initializeBoard();


  }

  void initializeBoard() {
    board = List.generate(3, (i) => List<String>.filled(3, ''));
    isPlayerX = true;
  }

  Future<void> saveBoardAsImage() async {
    RenderRepaintBoundary boundary = _boardKey.currentContext!.findRenderObject() as RenderRepaintBoundary;
    ui.Image image = await boundary.toImage();
    ByteData? byteData = await image.toByteData(format: ui.ImageByteFormat.png);
    Uint8List pngBytes = byteData!.buffer.asUint8List();

    
    // Save the image to the device's local storage (you can customize the path)
    String imagePath = await saveImageToLocalFile(pngBytes);
    print('Image saved at: $imagePath');
  }

  Future<String> saveImageToLocalFile(Uint8List bytes) async {
    //final appDir = await getTemporaryDirectory();
    final appDir = await getExternalStorageDirectory();
    final file = File('${appDir!.path}/tic_tac_toe_board.png');
    //await file.writeAsBytes(bytes);
    final result = await ImageGallerySaver.saveFile(file.path);
    if (result['isSuccess']) {
      print('Dosya galeriye kaydedildi: ${result['filePath']}');
    } else {
      print('Dosya galeriye kaydedilemedi.');
    }
    return file.path;
  }

void makeMove(int row, int col) {
    if (board[row][col] == '') {
      setState(() {
        board[row][col] = 'X';
        isPlayerX = false;
        if (checkForWinner()) {
          showWinnerDialog();
        } else if (isBoardFull()) {
          showDrawDialog();
        } else {
          makeAIMove(); // Make AI move after player
        }
      });
    }
  }

  void makeAIMove() {
    // Simple AI: Randomly choose an empty cell for 'O'
    List<int> emptyCells = [];
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (board[i][j] == '') {
          emptyCells.add(i * 3 + j);
        }
      }
    }

    if (emptyCells.isNotEmpty) {
      int randomIndex = Random().nextInt(emptyCells.length);
      int cell = emptyCells[randomIndex];
      int aiRow = cell ~/ 3;
      int aiCol = cell % 3;

      board[aiRow][aiCol] = 'O';
      isPlayerX = true;

      if (checkForWinner()) {
        showWinnerDialog();
      } else if (isBoardFull()) {
        showDrawDialog();
      }
    }
  }



  bool checkForWinner() {
    // Dikey, yatay ve çapraz kazanan kontrolü
    for (int i = 0; i < 3; i++) {
      if (board[i][0] != '' &&
          board[i][0] == board[i][1] &&
          board[i][1] == board[i][2]) {
        return true; // Dikey kontrol
      }
      if (board[0][i] != '' &&
          board[0][i] == board[1][i] &&
          board[1][i] == board[2][i]) {
        return true; // Yatay kontrol
      }
    }
    if (board[0][0] != '' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) {
      return true; // Soldan sağa çapraz kontrol
    }
    if (board[0][2] != '' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
      return true; // Sağdan sola çapraz kontrol
    }
    return false;
  }


  bool isBoardFull() {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (board[i][j] == '') {
          return false; // Hala boş hücre var
        }
      }
    }
    return true; // Tüm hücreler dolu
  }

void showWinnerDialog() {
  showDialog(
    context: context,
    builder: (BuildContext context) {
      return AlertDialog(
        title: Text('Kazanan'),
        content: Text(isPlayerX ? 'Oyuncu O kazandı!' : 'Oyuncu X kazandı!'),
        actions: <Widget>[
          TextButton(
            child: Text('Tamam'),
            onPressed: () {
              Navigator.of(context).pop();
              resetGame();
            },
          ),
        ],
      );
    },
  );
}

  void showDrawDialog() {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text('Berabere'),
          content: Text('Oyun berabere bitti.'),
          actions: <Widget>[
            TextButton(
              child: Text('Tamam'),
              onPressed: () {
                Navigator.of(context).pop();
                resetGame();
              },
            ),
          ],
        );
      },
    );
  }

  void resetGame() {
    setState(() {
      initializeBoard();
    });
  }

  Widget buildGameBoard() {
    List<Widget> rows = [];
    for (int i = 0; i < 3; i++) {
      List<Widget> rowChildren = [];
      for (int j = 0; j < 3; j++) {
        rowChildren.add(
          GestureDetector(
            onTap: () => makeMove(i, j),
            child: Container(
              width: 137.1,
              height: 137.1,
              decoration: BoxDecoration(
                border: Border.all(color: Colors.black), // Set border color to black
              ),
              child: Center(
                child: Stack(
                  children: [
                    // X işareti
                    if (board[i][j] == 'X')
                      NeonSign(
                        text: 'X',
                        color: Colors.black, // Set X color to black
                      ),
                    // O işareti
                    if (board[i][j] == 'O')
                      NeonSign(
                        text: 'O',
                        color: Colors.black, // Set O color to black
                      ),
                  ],
                ),
              ),
            ),
          ),
        );
      }
      rows.add(Row(children: rowChildren));
    }
    return Center(
      child: Column(children: rows),
    );
  }

@override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('XOX Game'),
      ),
      backgroundColor: Colors.black87,
      body: Container(
        decoration: BoxDecoration(),
        child: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.center,
            children: [
              RepaintBoundary(
                key: _boardKey,
                child: buildGameBoard(),
              ),
              SizedBox(height: 20),
              AppText(
                text: isPlayerX ? "Sıra X'in" : "Sıra O'nun",
                fontSize: 20.0,
                fontWeight: FontWeight.w600,
                color: Color(0xFF7C7C7C),
              ),
              ElevatedButton(
                onPressed: () => saveBoardAsImage(),
                child: Text('Save Board as Image'),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

class NeonSign extends StatelessWidget {
  final String text;
  final Color color;

  const NeonSign({
    required this.text,
    required this.color,
  });

  @override
  Widget build(BuildContext context) {
    return Stack(
      children: [
        // Mavi veya pembe gölge
        Positioned(
          top: 0,
          left: 0,
          child: Text(
            text,
            style: TextStyle(
              fontSize: 40.0,
              color: color,
              shadows: [
                Shadow(
                  blurRadius: 10.0,
                  color: color,
                  offset: Offset(0, 0),
                ),
              ],
            ),
          ),
        ),
        // Ana metin
        Text(
          text,
          style: TextStyle(
            fontSize: 40.0,
            color: Colors.white,
          ),
        ),
      ],
    );
  }
}
class FavouriteScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Container(
      child: Center(
        
        child: XOXGame(),
        
      ),
    );
  }
}
