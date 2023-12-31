import 'package:flutter/material.dart';
import 'package:flutter_svg/svg.dart';
import 'package:grocery_app/styles/colors.dart';
import 'package:provider/provider.dart';
import 'navigator_item.dart';
import 'dart:typed_data';
import 'dart:io';
import 'package:flutter/foundation.dart';
import 'package:grocery_app/helpers/drawing_state.dart';

class DashboardScreen extends StatefulWidget {
  late final String ipAddress;
  late final int port;

  DashboardScreen({required this.ipAddress, required this.port});

  @override
  _DashboardScreenState createState() => _DashboardScreenState();
}

class _DashboardScreenState extends State<DashboardScreen> {
  int currentIndex = 0;
  bool draw = false;
  bool isDrawing = false;
  late DrawingState drawingState;

  @override
  void initState(){
    super.initState();
    drawingState = Provider.of<DrawingState>(context, listen: false);
    connectionThread();
  }

  void connectionThread() async{
    try {
      final Directory tempDir = Directory.systemTemp;
      String imagePath = tempDir.path + '/' + "image.jpg";
      final chunkSize = 4095;
      Uint8List imageBytes = Uint8List(0);
      print("test1");


      //final socket = await Socket.connect("192.168.31.25", 8080);
      final socket = await Socket.connect(widget.ipAddress, widget.port);
      socket.write("0Connection Established");

      int i = 0;
      Uint8List chunkWithPrefix = Uint8List.fromList([]);
      int calculatedValue= 0;
      int totalLineNumberForAnImage = 0;
      socket.listen(
            (Uint8List data) {
          print("---------------------------------------\n");
          print('Sunucudan gelen veri: ${String.fromCharCodes(data)}');
          final serverResponse = Uint8List.fromList(data)[0] - 48;
          print("gelen komut -> $serverResponse");

          if (serverResponse == 0) {

            if(drawingState.draw){
              final imageFile = File(imagePath);
              imageBytes = imageFile.readAsBytesSync();
              print("Image length: ${imageBytes.length}");
              print("Image length in bytes: ${imageBytes.lengthInBytes}");
              print("Image size in KB: ${imageBytes.lengthInBytes / 1024}");
              print("Image size in MB: ${(imageBytes.lengthInBytes / 1024) / 1024}");
              if (i < imageBytes.length) {
                final end = (i + chunkSize < imageBytes.length)
                    ? i + chunkSize
                    : imageBytes.length;
                final chunk = imageBytes.sublist(i, end);

                // Add '1' at the beginning of each chunk, '2' for the last chunk
                final prefix = (end == imageBytes.length)
                    ? Uint8List.fromList([2])
                    : Uint8List.fromList([1]);
                chunkWithPrefix = Uint8List.fromList(prefix + chunk);

                socket.add(chunkWithPrefix);

                if(prefix.elementAt(0) == 2){
                  //draw = false;
                  drawingState.setDraw(false);
                }

                print('Sent chunk ${i ~/ chunkSize + 1}');
                i += chunkSize;
              }
              drawingState.setDrawing(true);
              //isDrawing = true;
            }
            else{
              socket.write("0Connection Established");
            }
          }
          else if (serverResponse == 1) {
            if (i < imageBytes.length) {
              final end = (i + chunkSize < imageBytes.length)
                  ? i + chunkSize
                  : imageBytes.length;
              final chunk = imageBytes.sublist(i, end);

              // Add '1' at the beginning of each chunk, '2' for the last chunk
              final prefix = (end == imageBytes.length)
                  ? Uint8List.fromList([2])
                  : Uint8List.fromList([1]);
              chunkWithPrefix = Uint8List.fromList(prefix + chunk);

              socket.add(chunkWithPrefix);

              if(prefix.elementAt(0) == 2){
                drawingState.setDraw(false);
                //draw = false;
              }
              print('Sent chunk ${i ~/ chunkSize + 1} with ${prefix.elementAt(0)} as prefix');
              i += chunkSize;
            }
          }
          else if (serverResponse == 2) {
            socket.write("6Connection Established");
          }
          else if (serverResponse == 3) {
            final serverResponseString = String.fromCharCodes(data);
            totalLineNumberForAnImage = int.parse(serverResponseString.substring(1));
            print("total line number: $totalLineNumberForAnImage");
            drawingState.setTotalLineNumber(totalLineNumberForAnImage);
            socket.write("6Connection Established");
          }
          else if (serverResponse == 4) {
            calculatedValue++;
            drawingState.setDrawnLineNumber(calculatedValue);
            if(calculatedValue == totalLineNumberForAnImage){
              drawingState.setDrawing(false);
              //isDrawing = false;
              calculatedValue = 0;
            }
            socket.write("6Connection Established");
          }
          else if (serverResponse == 5) {
            socket.write("6Connection Established");
          }
          else if (serverResponse == 6) {
            socket.write("6Connection Established");
          }

        },
        onError: (error) {
          print('Error: $error');
          //isDrawing = false;
          drawingState.setDrawing(false);
          socket.close();
        },
        onDone: () {
          print('Server closed connection.');
          drawingState.setDrawing(false);
          //isDrawing = false;
          socket.close();
        },
      );
    } catch (e) {
      print('Bağlantı hatası: $e');
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: navigatorItems[currentIndex].screen,
      bottomNavigationBar: Container(
        decoration: BoxDecoration(
          borderRadius: BorderRadius.only(
            topRight: Radius.circular(15),
            topLeft: Radius.circular(15), 
          ),
          boxShadow: [
            BoxShadow(
                color: Colors.black38.withOpacity(0.1),
                spreadRadius: 0,
                blurRadius: 37,
                offset: Offset(0, -12)),
          ],
        ),
        child: ClipRRect(
          borderRadius: BorderRadius.only(
            topLeft: Radius.circular(15),
            topRight: Radius.circular(15),
          ),
          child: BottomNavigationBar(
            backgroundColor: Colors.white,
            currentIndex: currentIndex,
            onTap: (index) {
              setState(() {
                currentIndex = index;
              });
            },
            type: BottomNavigationBarType.fixed,
            selectedItemColor: AppColors.primaryColor,
            selectedLabelStyle: TextStyle(fontWeight: FontWeight.w600),
            unselectedLabelStyle: TextStyle(fontWeight: FontWeight.w600),
            unselectedItemColor: Colors.black,
            items: navigatorItems.map((e) {
              return getNavigationBarItem(
                  label: e.label, index: e.index, iconPath: e.iconPath);
            }).toList(),
          ),
        ),
      ),
    );
  }

  BottomNavigationBarItem getNavigationBarItem(
      {required String label, required String iconPath, required int index}) {
    Color iconColor =
        index == currentIndex ? AppColors.primaryColor : Colors.black;
    return BottomNavigationBarItem(
      label: label,
      icon: SvgPicture.asset(
        iconPath,
        color: iconColor,
      ),
    );
  }
}
