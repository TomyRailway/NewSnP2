# 新型SN/P2プラグイン(D-ATC版)  
## 概要  
Unicorn氏が以前公開していた「JR東日本 次世代車両向け ATSプラグイン(通称Sn/P2プラグイン)」をベースにバグ修正、および新機能を追加したプラグインです。  

現在、開発に協力して頂ける方を募集しております。  
有用な機能は是非付属させたいと考えておりますので、[プルリク](https://github.com/TomyRailway/NewSnP2/pulls)を送っていただけますと幸いです。  
その他、実装してほしい機能等がありましたら、[issues](https://github.com/TomyRailway/NewSnP2/issues)を利用してご連絡ください

## プラグインの機能
ATS-SN・ATS-P・ATC-6・ATC-10・D-ATC・TIMS・EBなど  
今後順次機能を追加予定

## LICENSE
GNU General Public License v3.0が適用されます。  
従って当プラグインは、自由に実行し、動作を調べ、ソースコードを改変し、複製物を再頒布したり、ソースコードを改変したものを公開することができます。  

## 依存するライブラリ
### [BveEX(旧AtsEX)](https://github.com/automatic9045/BveEX) (PolyForm Noncommercial License 1.00)  
(BveExに含まれるAtsArrayExtenderを利用しています)

Copyright (c) 2022-2025 automatic9045

## 現在の最新バージョン
[Version1.03(2025/10/26版)](https://github.com/TomyRailway/NewSnp2/releases/tag/Version1.03)

## 端子台
端子台は[Googleスプレッドシート](https://docs.google.com/spreadsheets/d/1hismVojo9I8iO6cHoQmOT9DAAMhSxIcc)よりどうぞ

※こちらのファイルはD-ATC版です。ATC-10版をご利用の方は末尾にD-ATCが付いていない方のzipファイルをダウンロードしてください。

・今回の更新における変更点
1.コード修正によるクラッシュ問題の改善、およびプラグインの安定性向上
2.不要なファイルを整理し容量を削減
3.D-ATC版dllファイル・ソースコードを追加で公開
4.その他細かい修正