/*
2020.02
閾値統制用
ニオイ種類はAir, Odor1, Odor2
*/

import processing.serial.*;//シリアルライブラリを取り入れる

Serial port;
Questionnaire question;

int session = 1;//タスク番号
int task = 1;//刺激番号
boolean pointerFlag = false;

PrintWriter output;  // PrintWriter オブジェクトを格納する変数


/***************************************はじめの一回のみ行われる関数***************************************/
void setup() {  
  // 実体化
  question = new Questionnaire();
  
  // シリアルポートの設定
  port = new Serial(this,"COM3", 9600); //Arduino>ツール＞ボードから要確認！  
  
  // 画面の初期設定
  size(1950,1050);
  colorMode(RGB,256);
  background(0,0,0);
  frameRate(1000); 
  
  // データ書き込み関係
  output = createWriter("result/" +  month() + day() + hour() + minute() + "_result.txt");
  
  // 日本語表示するためのフォント設定
  PFont pf=createFont("SansSerif",12);
  textFont(pf);  
  
  // テキストファイルのタイトル行
  output.print("No,Stimulation,Intensity,Kassei,Hikassei,Kai,Fukai");
}  


/***********************frameRate(fps)だけ実行されるプログラム．メイン関数的なやつ．***********************/
void draw() {
// 閾値確認プログラム--------------------------------------------------------------
  switch(port.read()){
      
    // 初期安静
    case 'a':
      println("\nperss any key...\n P: preasant odor\n U: unpleasant odor\n N: odorless air\n");
      
      question.darkScreen();
      fill(255);
      noStroke();
      rectMode(CENTER);
      rect(width/2,height/2,20,120);
      rect(width/2,height/2,120,20);
      
      output.print("\n0,FIRSTQUESTION");
      port.clear();
      break;

    // 安静
    case 'b':
      println("\n安静中\n");
      question.darkScreen();
      fill(255);
      noStroke();
      rectMode(CENTER);
      rect(width/2,height/2,20,120);
      rect(width/2,height/2,120,20);
      port.clear();
      break;
      

    // 無臭
    case 'n':
      println("無臭提示 5sec...");
      
      question.darkScreen();
      fill(255);
      noStroke();
      rectMode(CENTER);
      rect(width/2,height/2,20,120);
      rect(width/2,height/2,120,20);
      
      output.print("\n" + task + ",Odorless");
      port.clear();
      task++;
      break;

    // 快臭
    case 'p':
      println("快臭提示 5sec...");
      
      question.darkScreen();
      
      image(loadImage("peach_photo.jpg"), 0, 0, 1950, 1050);
      fill(255);
      noStroke();
      rectMode(CENTER);
      rect(width/2,height/2,20,120);
      rect(width/2,height/2,120,20);
      
      output.print("\n" + task + ",Pleasant");
      port.clear();
      task++;
      break;
    
    // 不快臭
    case 'u':
      println("不快臭提示 5sec...");
      
      question.darkScreen();
      image(loadImage("boots_photo.jpg"), 0, 0, 1950, 1050);
      fill(255);
      noStroke();
      rectMode(CENTER);
      rect(width/2,height/2,20,120);
      rect(width/2,height/2,120,20);
      
      output.print("\n" + task + ",Unpleasant");
      port.clear();
      task++;
      break;
    
      
    // 閾値統制
    case 'c':
      println("閾値統制を行ってください\nENTERキーで次のステップへ\n");
      
      question.darkScreen();
      textAlign(CENTER);
      textSize(40);
      text("ニオイの閾値統制を行います\n提示したニオイが0から10でどの程度の強さかを答えてください", width/2, height/2+60);
      
      port.clear();
      break;


    // 構造画像撮影
    case 'd':
      println("構造画像撮影中...\nENTERキーで次のステップへ\n");
      
      question.darkScreen();
      
      port.clear();
      break;

    
    // ノイズキャンセリング設定
    case 'e':
      println("ノイズキャンセリング設定中...\nENTERキーで実験開始\n");
      
      question.darkScreen();
      
      port.clear();
      break;


    // ダミースキャン
    case 'f':
      println("\nダミースキャン10sec");
      
      question.darkScreen();
      
      port.clear();
      break;


    // アンケート開始前5sec画面
    case 'g':
      println("アンケートを始めます");
      
      question.darkScreen();
      textAlign(CENTER);
      textSize(50);
      fill(255);
      text("アンケートを始めます",width/2,height/3);
      text("赤文字の項目について回答してください",width/2,height/3+60);
      
      port.clear();
      break;
      
      
    //アンケート開始
    case 'h':
      question.startQuestion();
      
      port.clear();
      break;
      
      
    //アンケート結果の記録信号
    case 'i':
      question.saveResult();
      output.flush();
      
      port.clear();
      break;
      

    // session interval
    case 'j':
      println("カフ更正を行ってください\nENTERキーでダミースキャン開始\n");
      
      question.darkScreen();
      
      port.clear();
      break;
    
      
    // ダイヤル動作テスト
    case 's':
      println("実験前のダイヤルテスト動作"); 
      
      question.darkScreen();
      fill(255);
      textAlign(CENTER);
      textSize(50);
      text("実験前に，アンケートのテスト動作を行います",width/2,height/2);
      
      port.clear();
      break;


    case 't':       
      println("ダイヤルテスト中...");
      println("ENTERキーで実験を開始します．\n");
      
      question.darkScreen();
      fill(255);
      textAlign(CENTER);
      textSize(40);
      text("アンケート項目に当てはまる場合は100，そうでない場合は0です．",width/2,height/3-60);
      text("何か動作不良があれば教えてください．",width/2,height/3);
      text("問題ないようでしたら，実験を開始します．",width/2,height/3+60);
      pointerFlag=true;
      question.setVasAxis();
      
      port.clear();
      break;
  
    
    //修了信号
    case 'z':
      println("############################################");
      println("おしまい\n60秒間はチューブ内に空気を流します");
      println("############################################");
      
      question.darkScreen();
      textAlign(CENTER);
      textSize(50);
      fill(255);
      text("これで実験はすべて終了です．\n\nご協力ありがとうございました．",width/2,height/2);       
      
      output.close();
      port.clear();
      break;
    }
}

//キーボード入力時の行動--------------------------------------------------------------------------------------------
void keyPressed(){

  switch(key){
    
    // スタート信号の送信
    case ENTER:
      port.write('a');
      port.clear();//シリアルポートの初期化
      break;


    // ニオイテスト用
    case 'n':
      port.write('n');
      port.clear();
      break;
    
    case 'p':
      port.write('p');
      port.clear();
      break;
    
    case 'u':
      port.write('u');
      port.clear();
      break;


    // ダイヤル動作確認用
    case 's':
      port.write('s');
      port.clear();//シリアルポートの初期化
      break;
    
    // ダイヤル装置を回転させるとt, bが押下された判定がなされる
    // アンケートの解答(順方向)
    case 't':
      if(pointerFlag==true){
        question.movePointerRight();
      }else{
        question.setCenterPointer();
      }
      break;
    // アンケートの解答(逆方向)  
    case 'b':
      if(pointerFlag==true){
        question.movePointerLeft();
      }else{
        question.setCenterPointer();
      }
      break;
  }
}


/***********************************アンケートに関するクラス***********************************/
class Questionnaire{
  
  int questionNo = 0;
  double pointer = width/2, speed=2; //speedを小さくすると軸の動きが遅くなる
  
  
  //アンケート画面のリセット
  void darkScreen(){
    rectMode(CORNER);
    fill(0);
    rect(0, 0, width, height);
    pointer = width/2;
  }
  
  
  //アンケートの開始
  void startQuestion(){
    pointerFlag = true;
    this.darkScreen();
    rectMode(CENTER);
    textAlign(CENTER);
    textSize(40);
    fill(255);
    
    text("ニオイの強さ", width/3, height/3-100);
    text("目が覚めるようなニオイ", (width*2)/3, height/3-100);
    text("眠たくなるようなニオイ", (width)/4, height/3);
    text("いいニオイ", (width*2)/4, height/3);
    text("いやなニオイ", (width*3)/4, height/3);
        
    fill(255,0,0);
    switch(questionNo){
      case 0:
        text("ニオイの強さ", width/3, height/3-100);
        text("________________", width/3, height/3-100);
        println("0. ニオイの強さ");
        break;
        
      case 1:
        text("目が覚めるようなニオイ", (width*2)/3, height/3-100);
        text("__________________________", (width*2)/3, height/3-100);
        println("1. 活性");
        break;
        
      case 2:
        text("眠たくなるようなニオイ", (width)/4, height/3);
        text("__________________________", (width)/4, height/3);
        println("2. 非活性");
        break;
        
      case 3:
        text("いいニオイ", (width*2)/4, height/3);
        text("______________", (width*2)/4, height/3);
        println("3. 快");
        break;
        
      case 4:
        text("いやなニオイ", (width*3)/4, height/3);
        text("_____________", (width*3)/4, height/3);
        println("4. 不快");
        break;
    }
    
    this.setVasAxis();  
  }
 
 
  void movePointerRight(){
     if(pointer<(width/2)+400) 
       pointer += speed;
     if(pointer>=(width/2)+400) 
       pointer = (width/2)+400;
     
     this.setVasAxis();
   }
  
   
  void movePointerLeft(){
     if(pointer>(width/2)-400) 
       pointer -= speed;
     if(pointer<=(width/2)-400)
       pointer = (width/2)-400;
     
     this.setVasAxis();
   }
 
 
  //アンケート結果の記録
  void saveResult(){
    pointerFlag=false;
    double x;
    x = (pointer-((width/2)-400))/8;
    
    println("結果: " +x);
    output.print(","+x);
        
    if(questionNo < 4){
      questionNo++;
    }else{
      questionNo = 0;
    }
  }
  
  
  void resetQuestionNo(){
     questionNo=0; 
  }
  
  
  void setCenterPointer(){
     pointer=width/2;
  }


  void setVasAxis(){
    textSize(40);
    rectMode(CENTER);
    
    fill(0);
    rect(width/2, height*3/4, width, 150); 
    
    // white axis
    fill(255);
    noStroke();
    text(0, (width/2)-400, height*3/4-20);
    text(100, (width/2)+400, height*3/4-20); 
    rect(width/2, height*3/4, 800, 10);
    rect((width/2)-400, height*3/4, 5, 30);
    rect((width/2)+400, height*3/4, 5, 30); 
    
    // red bar
    fill(255,0,0);
    rect((int)pointer, height*3/4-20, 5, 30);
  }
}
