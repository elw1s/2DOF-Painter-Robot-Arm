import 'package:flutter/material.dart';
import 'app.dart';
import 'package:provider/provider.dart';
import 'helpers/drawing_state.dart';

import 'dart:typed_data';
import 'dart:io';

import 'package:flutter/foundation.dart';
void main() async {

  runApp(
    ChangeNotifierProvider(
      create: (context) => DrawingState(),
      child: MyApp(),
    ),
  );
  

  
  /*
  try {
    
  
    final socket = await Socket.connect("192.168.109.144", 8080);
    
    print("sa0");
    socket.write("0selamsapsik");
    print("sa1");

    socket.listen(
      (Uint8List data) {
        print('Sunucudan gelen veri: ${String.fromCharCodes(data)}');
      },
      onError: (error) {
        print('Hata oluştu: $error');
        socket.destroy();
      },
      onDone: () {
        print('Bağlantı kapandı');
        socket.destroy();
      },
    );

    // runApp çağrısı bağlantı kurulduktan sonra gerçekleşecek
  } catch (e) {
    // Bağlantı hatası durumunda kullanıcıya bildirim göster
    print('Bağlantı hatası: $e');

    
    // Burada kullanıcıya bir bildirim gösterebilirsiniz, örneğin:
    // showDialog veya snackbar kullanarak bir ileti gösterme
  }*/


}
