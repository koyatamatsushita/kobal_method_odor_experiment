/*
2020.02 @ author : Koyata Matsushita
2020.07.09 edited
    画像の変更，テキストの微修正

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
  size(2050,1150);
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
      println("\nperss any key...\n p: preasant odor\n u: unpleasant odor\n n: odorless air\n");
      
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
      println("無臭提示 15sec...");
      
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

    // ヘプタナール
    case 'q':
      println("ヘプタナール提示 15sec...");
      
      question.darkScreen();
      
      image(loadImage("oil.png"), 0, 0, 1950, 1050);
      fill(255);
      noStroke();
      rectMode(CENTER);
      rect(width/2,height/2,20,120);
      rect(width/2,height/2,120,20);
      
      output.print("\n" + task + ",Oldoil");
      port.clear();
      task++;
      break;
    
    // トリメチルベンゼン
    case 'w':
      println("トリメチルベンゼン提示 15sec...");
      
      question.darkScreen();
      image(loadImage("gasoline.png"), 0, 0, 1950, 1050);
      fill(255);
      noStroke();
      rectMode(CENTER);
      rect(width/2,height/2,20,120);
      rect(width/2,height/2,120,20);
      
      output.print("\n" + task + ",Gasoline");
      port.clear();
      task++;
      break;

    // ラクトン
    case 'p':
      println("ラクトン提示 15sec...");
      
      question.darkScreen();
      image(loadImage("peach.png"), 0, 0, 1950, 1050);
      fill(255);
      noStroke();
      rectMode(CENTER);
      rect(width/2,height/2,20,120);
      rect(width/2,height/2,120,20);
      
      output.print("\n" + task + ",Peach");
      port.clear();
      task++;
      break;

    // フェニルエチルアルコール
    case 'o':
      println("フェニルエチルアルコール提示 15sec...");
      
      question.darkScreen();
      image(loadImage("rose.png"), 0, 0, 1950, 1050);
      fill(255);
      noStroke();
      rectMode(CENTER);
      rect(width/2,height/2,20,120);
      rect(width/2,height/2,120,20);
      
      output.print("\n" + task + ",Rose");
      port.clear();
      task++;
      break;

    // メチルシクロペンテノロン
    case 'i':
      println("メチルシクロペンテノロン提示 15sec...");
      
      question.darkScreen();
      image(loadImage("caramel.png"), 0, 0, 1950, 1050);
      fill(255);
      noStroke();
      rectMode(CENTER);
      rect(width/2,height/2,20,120);
      rect(width/2,height/2,120,20);
      
      output.print("\n" + task + ",Caramel");
      port.clear();
      task++;
      break;

    // アンケート開始前5sec画面
    case 'x':
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
    case 'y':
      question.startQuestion();
      
      port.clear();
      break;
      
      
    //アンケート結果の記録信号
    case 'z':
      question.saveResult();
      output.flush();
      
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
    // odorless air
    case 'n':
      port.write('n');
      port.clear();
      break;
    
    // pleasant odor
    case 'p':
      port.write('p');
      port.clear();
      break;
    
    // unpleasant odor
    case 'u':
      port.write('u');
      port.clear();
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
    
    text("ニオイの強さ", width/4, height/3-100);
    text("活性度", (width*2)/4, height/3-100);
    text("非活性度", (width*3)/4, height/3-100);
    text("快度", (width)/4, height/3);
    text("不快度", (width*2)/4, height/3);
    text("ニオイの質", (width*3)/4, height/3);
        
    fill(255,0,0);
    switch(questionNo){
      case 0:
        text("ニオイの強さ", width/4, height/3-100);
        text("________________", width/4, height/3-100);
        println("0. ニオイの強さ");
        break;
        
      case 1:
        text("活性度", (width*2)/4, height/3-100);
        text("__________", (width*2)/4, height/3-100);
        println("1. 活性度");
        break;
        
      case 2:
        text("非活性度", (width*3)/4, height/3-100);
        text("____________", (width*3)/4, height/3-100);
        println("2. 非活性度");
        break;
        
      case 3:
        text("快度", (width)/4, height/3);
        text("________", (width)/4, height/3);
        println("3. 快度");
        break;
        
      case 4:
        text("不快度", (width*2)/4, height/3);
        text("__________", (width*2)/4, height/3);
        println("4. 不快度");
        break;

      case 5:
        text("ニオイの質", (width*3)/4, height/3);
        text("__________", (width*3)/4, height/3);
        println("5．ニオイの質");
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
        
    if(questionNo < 5){
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
    double parameter = (pointer - ((width/2)-500)) / 10;
    
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

    // 選択項目
    if(questionNo == 5){
      fill(0);
      text(0, (width/2)-500, height*3/4-20);
      text(100, (width/2)+500, height*3/4-20); 
      fill(255);
      text("ガソリン", (width/2)-400, height*3/4+50);
      text("青臭い", (width/2)-200, height*3/4+50);
      text("無臭", (width/2), height*3/4+50);
      text("果実", (width/2)+200, height*3/4+50);
      text("その他", (width/2)+400, height*3/4+50);
      rect((width/2)-300, height*3/4, 5, 20);
      rect((width/2)+300, height*3/4, 5, 20);
      rect((width/2)-100, height*3/4, 5, 20);
      rect((width/2)+100, height*3/4, 5, 20);
      
      // 選択項目の赤色表示
      fill(255,0,0);
      if( 0 <= parameter && parameter < (((width/2)-300)-((width/2)-500))/10 ){
        
        text("ガソリン", (width/2)-400, height*3/4+50);
        
      }else if( (((width/2)-300)-((width/2)-500))/10 <= parameter && parameter < (((width/2)-100)-((width/2)-500))/10 ){
        
        text("青臭い", (width/2)-200, height*3/4+50);
        
      }else if( (((width/2)-100)-((width/2)-500))/10 <= parameter && parameter < (((width/2)+100)-((width/2)-500))/10 ){
        
        text("無臭", (width/2), height*3/4+50);
        
      }else if( (((width/2)+100)-((width/2)-500))/10 <= parameter && parameter < (((width/2)+300)-((width/2)-500))/10 ){
        
        text("果実", (width/2)+200, height*3/4+50);
        
      }else{
        
        text("その他", (width/2)+400, height*3/4+50);
        
      }
    } else {
      fill(0);
      text("ガソリン", (width/2)-400, height*3/4+50);
      text("青臭い", (width/2)-200, height*3/4+50);
      text("無臭", (width/2), height*3/4+50);
      text("果実", (width/2)+200, height*3/4+50);
      text("その他", (width/2)+400, height*3/4+50);
    }
  }
}
