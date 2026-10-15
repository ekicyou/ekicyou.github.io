// サイトに 1 つだけの値を、1 か所に持つ。部品・レイアウト・ルート・フィードは、ここの `SITE` を読む。
// サイト名・説明文・作者・リンク・giscus の値は、切り替えの前の Jekyll の設定と、giscus の埋め込みから写した。
// 作者への連絡先は、GitHub・Mastodon・コメント欄だけにする。ほかの連絡先の項目は持たない。
// `astro` の機能に依らない(スクリプトや単体テストから直に読める)。

export interface NavItem {
  label: string;
  href: string;
}

export interface SiteConfig {
  url: string; // 末尾の `/` なし。絶対 URL は、ここに公開パスをつないで作る
  title: string;
  description: string;
  author: string;
  lang: 'ja';
  ogLocale: 'ja_JP';
  icon: string; // サイトの顔と、既定の SNS 向けの画像
  favicon: string;
  feedPath: '/feed.xml';
  fontsHref?: string; // Google Fonts のスタイルシートの URL
  nav: readonly NavItem[];
  links: { github: string; mastodon: string };
  giscus: Readonly<Record<string, string>>; // giscus のスクリプトに渡す data-* の値
}

// 今ある絵。飾りに使う絵は、これ 1 枚だけである。
const ICON = '/img/alto0044_400x400.png';

export const SITE: SiteConfig = {
  url: 'https://ekicyou.github.io',
  title: 'えちょのざんがい',
  // 切り替えの前のサイトが出していた文と、1 字も違えない。文の間の空白は、元の設定の改行の跡である。
  description:
    'このサイトは２０世紀後半に発生した、 えちょの嗜好についての諸々をgithubが続く限り残し続ける場所です。 世界のエントロピー上昇に貢献します。',
  author: 'ekicyou',
  lang: 'ja',
  ogLocale: 'ja_JP',
  icon: ICON,
  // favicon は、アイコン絵を指す。新しい絵柄は作らない。
  favicon: ICON,
  feedPath: '/feed.xml',
  // 見出しの DotGothic16 と、本文の M PLUS 1p(400・500・700)。
  fontsHref:
    'https://fonts.googleapis.com/css2?family=DotGothic16&family=M+PLUS+1p:wght@400;500;700&display=swap',
  // ヘッダーのメニュー。初めは About だけ。
  nav: [{ label: 'About', href: '/about.html' }],
  links: {
    github: 'https://github.com/ekicyou',
    mastodon: 'https://ukadon.shillest.net/@ekicyou',
  },
  // 切り替えの前の埋め込みと同じ 12 項目。項目の名前は、スクリプトの属性の名前そのままにする。
  // リポジトリ・カテゴリ・`data-mapping` を変えると、ページとコメントの対応が切れる。
  giscus: {
    'data-repo': 'ekicyou/ekicyou.github.io',
    'data-repo-id': 'MDEwOlJlcG9zaXRvcnk2OTMzMzcz',
    'data-category': 'Announcements',
    'data-category-id': 'DIC_kwDOAGnLfc4CzVb5',
    'data-mapping': 'pathname',
    'data-strict': '0',
    'data-reactions-enabled': '1',
    'data-emit-metadata': '0',
    'data-input-position': 'bottom',
    'data-theme': 'preferred_color_scheme',
    'data-lang': 'ja',
    'data-loading': 'lazy',
  },
};
