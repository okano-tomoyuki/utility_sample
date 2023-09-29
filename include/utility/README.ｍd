# Utility名前空間

Utility名前空間は、システム開発全体を通じて再利用性の高い関数クラスを管理するための名前空間である。
利用対象はC++11以降のコンパイラを使用していることを想定している。また、対象プラットフォームについてはWindows/Linux環境を想定している。

---

## 関数一覧

|関数名|概要|
|--|--|
|split|文字列分割関数|
|concat|文字列連結関数|
|range|範囲ベース forループ用関数|

### split関数

split関数は引数の原始文字列を特定の区切り文字で分割したベクターを返却する関数である。

- オーバーロードなし
- 例外を送出しない

|項目|値|説明|
|--|--|--|
|引数1|const std::string& original|原始文字列|
|引数2|const char& separator=','|区切り文字(デフォルトは',')|
|返値|std::vector\<std::string>|原始文字列を区切り文字で分割した文字列のベクター|

使用例

``` cpp
using split = Utility::split;

std::string original_str = "hello,my,name,is,tanaka";
auto split_str = split(original_str);
for(const auto& str: split_str)
    std::cout << str << std::endl;

/**
 * 出力結果
 * hello
 * my
 * name
 * is
 * tanaka
**/

std::string original_str = "hello my name is tanaka";
auto split_str = split(original_str, ' ');
for(const auto& str: split_str)
    std::cout << str << std::endl;

/**
 * 出力結果
 * hello
 * my
 * name
 * is
 * tanaka
**/

```

---

### concat関数

- オーバーロードなし
- 例外を送出しない

concat関数は引数のベクターデータを特定の区切り文字で連結した文字列を返却する関数である。

|項目|値|説明|
|--|--|--|
|引数1|const std::vector\<T>& original|プリミティブ型のベクターデータ|
|引数2|const char& separator=','|区切り文字(デフォルトは',')|
|返値|std::string|原始文字列を区切り文字で分割した文字列のベクター|

使用例

``` cpp
using concat = Utility::concat;

std::vector<std::string> vector_str;
vector_str.push_back("hello");
vector_str.push_back("my");
vector_str.push_back("name");
vector_str.push_back("is");
vector_str.push_back("tanaka");

std::cout << concat(vector_str) << std::endl;

/**
 * 出力結果
 * hello,my,name,is,tanaka
**/

std::vector<int> vector_int;
vector_str.push_back(10);
vector_str.push_back(20);
vector_str.push_back(30);
vector_str.push_back(40);

std::cout << concat(vector_int, ' ') << std::endl;

/**
 * 出力結果
 * 10 20 30 40
**/

```

---

### range関数

- オーバーロードあり
- 例外を送出しない

range関数は範囲ベースforループを利用する際に使用する。
範囲ベースforループについての詳細は以下を参照のこと。

[link](https://cpprefjp.github.io/lang/cpp11/range_based_for.html)

本関数はpythonにおけるrange関数と同等の動作を期待する。

|項目|値|説明|
|--|--|--|
|引数1|const T& end|int型にキャスト可能なデータ|
|返値|std::vector\<int>|範囲ベースforループ用のインデックス|

|項目|値|説明|
|--|--|--|
|引数1|const int& start|インクリメント開始値|
|引数2|const int& end|インクリメント終了値|
|引数2|const int& interval|インデックスのインクリメント値|
|返値|std::vector\<int>||

使用例

``` cpp
using range = Utility::range;

for(const auto& i : range(5))
    std::cout << i << std::endl;

/**
 * 出力結果
 * 0
 * 1
 * 2
 * 3
 * 4
**/

for(const auto& i : range(5, 10))
    std::cout << i << std::endl;

/**
 * 出力結果
 * 5
 * 6
 * 7
 * 8
 * 9
**/

for(const auto& i : range(1, 18, 4))
    std::cout << i << std::endl;

/**
 * 出力結果
 * 1
 * 5
 * 9
 * 13
 * 17
**/

```

---

## クラス一覧

Utility名前空間内で利用するクラスは主にSocket通信や共有メモリの管理等利用頻度の高い普遍的な処理を抽象化し、コードの再生産性を高めることを目指す。

実装済クラス一覧
|クラス名|概要|
|--|--|
|SharedMemory|共有メモリ管理クラス(Memory Maped File)|

---

### SharedMemoryクラス

概要
SharedMemoryクラスは親子関係のないプロセス及びスレッド間で使用する共通変数についてMemory Maped Fileによる共有メモリオブジェクトを管理するためのクラスである。
本クラスを利用することによりマップファイルの生成、マップハンドルの生成/破棄、及び排他制御処理の処理を全て一任することができる。
排他制御についてはWindows APIにおけるWaitForSingleObject関数がミリ(1e-3)秒単位でしか制御できないのに対し、ナノ(1e-9)秒単位で制御を実現する。
本クラスはCompositWionによる利用を想定し、継承を許容しない。

クラスメンバ
|メンバ|種別|説明|
|--|--|--|
|data_|フィールド|対象データポインタ|
|buffer_size_|フィールド|対象データのサイズ|
|map_handle_|フィールド|マップのハンドル|
|try_read|メソッド|データ書込試行処理。処理に失敗した場合はfalseを返す。タイムアウトは小数点以下を含むミリ秒とする。デフォルト値は1ミリ秒。|
|try_write|メソッド|データ読込試行処理。処理に失敗した場合はfalseを返す。タイムアウトは小数点以下を含むミリ秒とする。デフォルト値は1ミリ秒。|
|flush|メソッド|一番最初に起動するプロセスにて使用。前回実行時のゴミデータを初期化する。|

``` mermaid
classDiagram

    class ClientA {　
        DataStruct* data
        SharedMemory DataStruct shmem_data
    }

    class ClientB {
        DataStruct* data
        SharedMemory DataStruct shmem_data     
    }

    class ClientC {
        DataStruct* data
        SharedMemory DataStruct shmem_data    
    }

    SharedMemory o--> ClientA
    DataStruct o--> ClientA

    SharedMemory o--> ClientB
    DataStruct o--> ClientB

    SharedMemory o--> ClientC
    DataStruct o--> ClientC

    class DataStruct {
        + hoge : Hoge
        + fuga : Fuga
        + piyo : Piyo
    }

    class SharedMemory {
        - char* data_ : 対象のデータポインタ
        - int buffer_size_ : データバッファサイズ
        - MapHandle map_handle_ : データハンドル Windows:HANDLE/Linux:int
        + SharedMemory<T>(const char* mutex_file_path) : コンストラクタ
        + bool try_read(T* data, const double& timeout) : メモリ読取試行
        + bool try_write(const T* data, const double& timeout) : メモリ書込試行
        + void flush() : ゴミデータ破棄処理
    }

```

詳細実装
インスタンス生成時に対象変数の構造体サイズに加え1バイトを加えたバッファー領域を確保し、余分に加えた1バイト分をMutex領域として利用することにより排他制御を実現している。

