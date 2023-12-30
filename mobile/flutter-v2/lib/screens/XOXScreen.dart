import 'package:flutter/material.dart';
import 'dart:math';
import 'dart:ui' as ui;
import 'dart:typed_data';
import 'package:image/image.dart' as img;

class XOXScreen extends StatefulWidget {
  @override
  _XOXScreenState createState() => _XOXScreenState();
}

class _XOXScreenState extends State<XOXScreen> {
  late List<List<String>> grid;
  late bool isPlayerTurn;

  @override
  void initState() {
    super.initState();
    initializeGame();
  }

  void initializeGame() {
    grid = List.generate(3, (_) => List.generate(3, (_) => ''));
    isPlayerTurn = true;
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Tic Tac Toe'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            AspectRatio(
              aspectRatio: 1.0,
              child: Container(
                color: Colors.white,
                child: GridView.builder(
                  itemCount: 9,
                  gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
                    crossAxisCount: 3,
                  ),
                  itemBuilder: (BuildContext context, int index) {
                    int row = index ~/ 3;
                    int col = index % 3;
                    return GestureDetector(
                      onTap: () {
                        if (grid[row][col] == '' && isPlayerTurn) {
                          setState(() {
                            grid[row][col] = 'X';
                            isPlayerTurn = false;
                            checkGameStatus();
                            if (!isPlayerTurn) {
                              makeAIMove();
                            }
                          });
                        }
                      },
                      child: Container(
                        decoration: BoxDecoration(
                          border: Border.all(color: Colors.black),
                        ),
                        child: Center(
                          child: Text(
                            grid[row][col],
                            style: TextStyle(fontSize: 40.0),
                          ),
                        ),
                      ),
                    );
                  },
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  void makeAIMove() {
    List<int> emptyCells = [];
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (grid[i][j] == '') {
          emptyCells.add(i * 3 + j);
        }
      }
    }

    if (emptyCells.isNotEmpty) {
      Random random = Random();
      int randomIndex = random.nextInt(emptyCells.length);
      int cell = emptyCells[randomIndex];
      int row = cell ~/ 3;
      int col = cell % 3;

      setState(() {
        grid[row][col] = 'O';
        isPlayerTurn = true;
        checkGameStatus();
      });
    }
  }

  void checkGameStatus() {
    String winner = '';
    // Check rows
    for (int i = 0; i < 3; i++) {
      if (grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2] && grid[i][0] != '') {
        winner = grid[i][0];
      }
    }
    // Check columns
    for (int i = 0; i < 3; i++) {
      if (grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i] && grid[0][i] != '') {
        winner = grid[0][i];
      }
    }
    // Check diagonals
    if ((grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2] && grid[0][0] != '') ||
        (grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0] && grid[0][2] != '')) {
      winner = grid[1][1];
    }

    if (winner.isNotEmpty || isGridFull()) {
      showDialog(
        context: context,
        builder: (_) => AlertDialog(
          title: Text('Game Over'),
          content: winner.isNotEmpty ? Text('Winner: $winner') : Text('It\'s a draw!'),
          actions: <Widget>[
            TextButton(
              onPressed: () {
                Navigator.of(context).pop();
                initializeGame();
              },
              child: Text('Play Again'),
            ),
          ],
        ),
      );
    }
  }

  bool isGridFull() {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (grid[i][j] == '') {
          return false;
        }
      }
    }
    return true;
  }
}