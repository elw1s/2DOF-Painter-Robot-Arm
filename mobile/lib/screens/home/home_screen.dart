import 'package:flutter/material.dart';
import 'dart:typed_data';
import 'dart:io';

import 'package:image_picker/image_picker.dart';
import 'package:flutter/foundation.dart';
import 'package:provider/provider.dart';

import '../../helpers/drawing_state.dart';

class HomeScreen extends StatefulWidget {
  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  File? _image;
  late DrawingState drawingState;
  //bool draw = false;
  //bool isDrawing = false;

  /*@override
  void initState(){
    super.initState();
    connectionThread();
  }*/

  @override
  void initState(){
    super.initState();
    drawingState = Provider.of<DrawingState>(context, listen: false);
  }

  Future getImage(ImageSource source) async {
    final picker = ImagePicker();
    final pickedFile = await picker.pickImage(    source: source,
        imageQuality: 10, // Adjust the image quality here (0-100)
    );
    String imagePath = ''; // New variable to hold the temporary file path

    setState(() {
      if (pickedFile != null) {
        _image = File(pickedFile.path);

        // Saving the image to a temporary location with the name "image.jpg"
        final Directory tempDir = Directory.systemTemp;
        final String fileName = 'image.jpg'; // Updated file name
        final File tempImage = File('${tempDir.path}/$fileName');
        tempImage.writeAsBytesSync(_image!.readAsBytesSync());
        imagePath = tempImage.path; // Updating the imagePath variable
        //draw = true;
      } else {
        print('No image selected.');
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      home: Scaffold(
        appBar: AppBar(
          title: Text('Draw Image'),
        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              Stack(
                children: [
                  Container(
                    height: 20,
                    width: 300,
                    child: LinearProgressIndicator(
                      backgroundColor: Color(0xFF33C2FF),
                      value: (drawingState.totalLineNumber == 0
                          ? 0.0
                          : (drawingState.drawnLineNumber / drawingState.totalLineNumber) * 100) / 100,
                      valueColor: AlwaysStoppedAnimation<Color>(Colors.blue), // Change color if needed
                    ),
                  ),
                  Positioned(
                    right: 0,
                    child: Container(
                      padding: EdgeInsets.symmetric(horizontal: 8),
                      color: Colors.transparent,
                      child: Text(
                        '${(drawingState.totalLineNumber == 0
                            ? 0.0
                            : (drawingState.drawnLineNumber / drawingState.totalLineNumber) * 100).toInt()}%', // Display the percentage value
                        style: TextStyle(color: Colors.black),
                      ),
                    ),
                  ),
                ],
              ),
              SizedBox(height: 20),
              _image == null ? Text('Select an image to draw') : Image.file(_image!),
              SizedBox(height: 20),
              ElevatedButton(
                onPressed: () {
                  getImage(ImageSource.gallery);
                },
                child: Text('Gallery'),
              ),
              ElevatedButton(
                onPressed: () {
                  getImage(ImageSource.camera);
                },
                child: Text('Photo'),
              ),
            ],
          ),
        ),
        floatingActionButton: FloatingActionButton(
          onPressed: drawingState.isDrawing ? null : () {
            setState(() {
              drawingState.draw = true;
              drawingState.isDrawing = true;
            });
          },
          child: Icon(Icons.draw),
          backgroundColor: (drawingState.isDrawing || (_image == null && !drawingState.isDrawing))
              ? Color(0xFF4F4F4F)
              : Color(0xFF33C2FF),
          elevation: (drawingState.isDrawing || (_image == null && !drawingState.isDrawing)) ? 0 : 6,
        ),
        floatingActionButtonLocation: FloatingActionButtonLocation.endFloat,
      ),
    );
  }
}
