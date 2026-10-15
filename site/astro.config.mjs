// サイトの設定。出力の形、Markdown の処理系とプラグイン、コードの配色を、ここ 1 か所で決める。
import { defineConfig } from 'astro/config';
import { unified } from '@astrojs/markdown-remark';
import remarkCjkFriendly from 'remark-cjk-friendly';
import remarkMath from 'remark-math';
import { remarkAlert } from 'remark-github-blockquote-alert';
import rehypeKatex from 'rehype-katex';

// KaTeX のスタイルシートから woff と ttf の指定を落とし、woff2 だけを公開物に出す。
// スタイルシートの書き方が変わって効かなくなっても、フォントが余分に出るだけで、表示は崩れない。
const katexWoff2Only = {
  name: 'katex-woff2-only',
  enforce: 'pre',
  transform(code, id) {
    if (!id.includes('katex/dist/katex.min.css')) return undefined;
    return code.replace(/,url\([^)]*\.(?:woff|ttf)\) format\("(?:woff|truetype)"\)/g, '');
  },
};

// 本文のメールアドレスが、自動でリンク(mailto:)になるのを止め、文字のまま出す。
function rehypeNoMailto() {
  const walk = (node) => {
    if (!node.children) return;
    node.children = node.children.flatMap((child) =>
      child.type === 'element' && child.tagName === 'a' && String(child.properties?.href ?? '').startsWith('mailto:')
        ? child.children
        : [child],
    );
    node.children.forEach(walk);
  };
  return (tree) => walk(tree);
}

// KaTeX に無い命令を、記号として定義する。
const katexMacros = {
  '\\lambdabar': '{\\lambda\\mkern-9mu\\raisebox{0.35ex}{-}}',
};

export default defineConfig({
  // 公開済みの URL を保つ形。ページはファイル単位(/about.html)で出し、末尾の / を付けない。
  // base は置かない。統合(サイトマップなど)は入れない。
  site: 'https://ekicyou.github.io',
  build: { format: 'file' },
  trailingSlash: 'never',
  vite: {
    plugins: [katexWoff2Only],
  },
  markdown: {
    // 従来の処理系を明示して選ぶ。プラグインはここへ渡す。GFM と見出しの id は、既定のまま使う。
    processor: unified({
      // 順番を変えない: 日本語の太字 → 数式の読み取り → 囲み。
      remarkPlugins: [remarkCjkFriendly, remarkMath, remarkAlert],
      // 数式はビルド時に描く。読めない数式は、ビルドを止めず、TeX の文字のまま出す。
      rehypePlugins: [rehypeNoMailto, [rehypeKatex, { strict: false, macros: katexMacros }]],
    }),
    // 配色を 2 つ渡し、色を CSS の変数(--shiki-light・--shiki-dark)として出させる。
    // どちらを使うかは、スタイルシートが決める。
    shikiConfig: {
      themes: { light: 'vitesse-light', dark: 'vitesse-dark' },
      defaultColor: false,
    },
  },
});
