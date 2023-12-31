import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'dart:math';
import 'dart:ui' as ui;
import 'dart:typed_data';
import 'package:image/image.dart' as img;
import 'package:provider/provider.dart';

import '../helpers/drawing_state.dart';

class XOXScreen extends StatefulWidget {
  @override
  _XOXScreenState createState() => _XOXScreenState();
}

class _XOXScreenState extends State<XOXScreen> {
  late List<List<String>> grid;
  late bool isPlayerTurn;
  late int lastMoveUser_row;
  late int lastMoveUser_col;
  late int lastMoveAI_row;
  late int lastMoveAI_col;
  late DrawingState drawingState;
  late bool isFirstMoveDone;

  @override
  void initState() {
    super.initState();
    drawingState = Provider.of<DrawingState>(context, listen: false);
    initializeGame();
  }

  void initializeGame() {
    grid = List.generate(3, (_) => List.generate(3, (_) => ''));
    isPlayerTurn = true;
    isFirstMoveDone = false;
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Tic Tac Toe'),
      ),
      body: Column(
        children: [
          LinearProgressIndicator(
            value: drawingState.totalLineNumber == 0
                ? 0.0
                : drawingState.drawnLineNumber / drawingState.totalLineNumber,
          ),
          Expanded(
            child: Center(
              child: AspectRatio(
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
                          if (drawingState.isDrawing) {
                            // Show dialog indicating drawing is in progress
                            showDialog(
                              context: context,
                              builder: (_) => AlertDialog(
                                title: Text('ERROR'),
                                content: Text('While the robot is drawing, you cannot play XOX.'),
                                actions: <Widget>[
                                  TextButton(
                                    onPressed: () {
                                      Navigator.of(context).pop();
                                    },
                                    child: Text('OK'),
                                  ),
                                ],
                              ),
                            );
                          } else if (grid[row][col] == '' && isPlayerTurn) {
                            setState(() {
                              grid[row][col] = 'X';
                              lastMoveUser_row = row;
                              lastMoveUser_col = col;
                              isPlayerTurn = false;
                              checkGameStatus();
                              if (!isPlayerTurn) {
                                makeAIMove();
                              }
                              saveImage();
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
            ),
          ),
        ],
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
        lastMoveAI_row = row;
        lastMoveAI_col = col;
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

  Future<void> saveImage() async {
    // Get image paths for user's and AI's moves from assets
    String tableImagePath;
    int numberOfImagesToAdd;
    if (!isFirstMoveDone) {
      tableImagePath = "assets/images/xox_images/table.jpg";
      numberOfImagesToAdd = 3;
    } else {
      tableImagePath = ""; // Set this to an empty string or any default value when isFirstMoveDone is true
      numberOfImagesToAdd = 2;
    }

    // Load user and AI images using rootBundle
    List<Uint8List> imagesBytes = [];
    for (int i = 0; i < numberOfImagesToAdd; i++) {
      String imagePath;
      print('isFirstMoveDone: $isFirstMoveDone');
      if (i == 2 && !isFirstMoveDone) {
        imagePath = tableImagePath;
      } else {
        imagePath = "assets/images/xox_images/${i == 0 ? 'x_' : 'o_'}${i == 0 ? lastMoveUser_row : lastMoveAI_row}${i == 0 ? lastMoveUser_col : lastMoveAI_col}.jpg";
      }

      print('Image Path: $imagePath'); // Print image paths for debugging

      ByteData? imageData = await rootBundle.load(imagePath);
      if (imageData != null) {
        imagesBytes.add(imageData.buffer.asUint8List());
      }
    }

    // Check if the images are successfully loaded
    if (imagesBytes.length == numberOfImagesToAdd) {
      List<img.Image> images = imagesBytes.map((bytes) => img.decodeImage(bytes)!).toList();

      // Ensure all images have the same dimensions
      bool dimensionsMatch = images.every((image) => image.width == images[0].width && image.height == images[0].height);
      if (dimensionsMatch) {
        // Create a canvas for the combined image
        img.Image combinedImage = img.Image(images[0].width, images[0].height);

        // Iterate through each pixel and blend corresponding pixel values
        for (int y = 0; y < images[0].height; y++) {
          for (int x = 0; x < images[0].width; x++) {
            List<int> pixelValues = [];
            for (int i = 0; i < numberOfImagesToAdd; i++) {
              pixelValues.add(images[i].getPixel(x, y));
            }

            List<int> combinedPixelValues = [];
            for (int c = 0; c < 3; c++) {
              int sum = 0;
              for (int i = 0; i < numberOfImagesToAdd; i++) {
                sum += img.getRed(pixelValues[i]);
              }
              combinedPixelValues.add((sum / numberOfImagesToAdd).round());
            }

            int combinedPixel = img.getColor(combinedPixelValues[0], combinedPixelValues[1], combinedPixelValues[2]);
            combinedImage.setPixel(x, y, combinedPixel);
          }
        }

        // Convert combined image to bytes
        Uint8List combinedBytes = Uint8List.fromList(img.encodeJpg(combinedImage));

        // Display the combined image in a dialog
        try {
          // Get the temporary directory path
          final Directory tempDir = Directory.systemTemp;
          final String fileName = 'image.jpg'; // Updated file name
          // Create a file with the name image.jpg in the temporary directory
          File imageFile = File('${tempDir.path}/image.jpg');

          // Write the combined image bytes to the file
          imageFile.writeAsBytesSync(combinedBytes);
          drawingState.setDraw(true);
          // Inform the user that the image has been saved
          print('Image saved to temporary folder: ${imageFile.path}');
        } catch (e) {
          // Handle any errors that occur during file operations
          print('Error saving image: $e');
        }
      } else {
        // Handle images with different dimensions
        print('Images have different dimensions');
      }
    } else {
      // Handle if images are not loaded
      print('Failed to load images from assets');
    }

    if(!isFirstMoveDone){
      isFirstMoveDone = true;
    }

  }


}