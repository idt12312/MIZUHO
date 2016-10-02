# 内容物
* pcb/ : kicadの基板データ
* firmware/ : stm32のファームウェア(eclipseのプロジェクトファイル)

# 開発環境
## 基板
* 基板の外形はinventorで作成 -> dxfで出力
* 回路図・アートワークはKiCad 4.0.4-stableで作成

## STM32
* コンパイラ : GCC 4.9.3 (arm-none-eabi)
* デバッガ : GDB 7.8.0.20150604-cvs (arm-none-eabi)
* OpenOCD 0.9.0
* IDE : Eclipse
  * CDT
  * GNU ARM Eclipse plugin
* eclipseのプロジェクトの雛形はGNU ARM Eclipse pluginから自動生成されるものを使用
* STM32のStandard Peripheral Libraryを使用

eclipseに上記pluginをinstallするとfirmwareディレクトリ以下のeclipseのプロジェクトが読み込めるようになる. コンパイラなどのツールのpathを適宜設定するとビルド・デバッグができるようになる.
