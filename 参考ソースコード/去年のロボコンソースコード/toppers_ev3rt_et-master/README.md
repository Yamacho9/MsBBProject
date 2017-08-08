[![Build Status](https://travis-ci.org/tak256real/toppers_ev3rt_et.svg?branch=master)](https://travis-ci.org/tak256real/toppers_ev3rt_et)


# toppers_ev3_et

## ビルドの仕組み

1. workspace/Makefile で ターゲットの指定(以下 target とする)
2. workspace/target/Makefile.inc を include し、 configure のパラメータを設定する
3. ディレクトリ workspace/OBJ を 作成し、 作成した OBJ を カレントディレクトリにする
4. OBJ/ 内で configureを実行、Makefileを⽣成
   - workspace/target をパスに含めて，2の設定を使⽤
   - make app=xxx の場合のテンプレートは Makefile.app
5. OBJ/ で 4. で⽣成した Makefile を make
6. スタンドアローン版の場合は，objcopy で ELF形式のモジュールからバイナリ形式の hrp2.bin を⽣成し、mkimage コマンドで hrp2.bin から uImage を⽣成
   - uboot がロードするためのファイル形式に変換
7. workspace/ に app or uImage をコピー
   - appは動的ローディング形式のアプリケーションモジュール

## 新規プロジェクトの作成

1. 始めに既存のプロジェクトをコピーすると楽
   - cp –a old-target new-terget
2. 不要なソースコードを削除し、必要なファイルを追加する
   - app.c / app.cpp / app.cfg あたりはデフォルトだと必要なファイルなので削除はしない事
   - 上記、 app から名称を変更する場合は workspase/Makefile の修正が必要
3. Makefile.inc でアプリケーションの設定
   - APPL_COBJS += xxx.o として gcc のビルド対象を追加
   - APPL_CXXOBJS += xxx.o として g++ のビルド対象を追加
   - SRCLANG := c | c++ で C か C++ もありかを設定する

