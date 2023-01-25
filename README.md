# Kodatuno

## Kodatuno について
Kodatuno は，金沢大学マンマシン研究室で開発されているオープンソースのCAMカーネルです．
本家は <http://www-mm.hm.t.kanazawa-u.ac.jp/research/kodatuno/>

## 本リポジトリについて
**金沢大学マンマシン研究室より特別に許可を得て**こちらのOrganizationで公開しています．
将来的にリポジトリのURLが変更になると思いますので，ご了承ください．  
Gitツリーは Kodatuno の更新履歴から再構築しました．

## 各ブランチについて
main ブランチは，本家からダウントードしたソースコードでコミットしたものです．  
Visual Studio ブランチは，Visual Studio 2022 でビルドできるようにしたソリューション構成で，Kodatunoカーネルのスタティックライブラリ（Kodatuno.vs.lib）を生成します．
Kodatuno.vs フォルダ配下に本家の Src/GE フォルダにあるコード一式をコピーしています．
シンボリックリンクにしたいところですが， Git for Windows ではオプションを設定しないと使えないのでコピーしています．

## Visual Studio でのビルド方法
Visual Studio ブランチをチェックアウトし，Kodatuno.slh を開いてください．
無料版の Visual Studio 2022 Community でもビルドできます．  
生成した Kodatuno.vs.lib の利用方法は，
NCVC のビルド方法 <https://github.com/NCVC-CAM/NCVC/blob/main/docs/build.md> を参考にしてください．
