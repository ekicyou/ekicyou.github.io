# えちょのざんがい

個人ブログ「えちょのざんがい」の原稿。公開先は <https://ekicyou.github.io/>。

- 生成ツールは [Astro](https://astro.build/)。原稿と仕組みは `site/`(Astro のプロジェクト)にある。記事は `site/src/content/posts/`、文書は `site/src/content/doc/`、画像などそのまま公開するファイルは `site/public/`。ルートにあるのは開発用のファイルで、公開されない。
- 手元で見るには、Node.js 26 を入れて、`site/` で `npm ci` → `npm run dev` を実行し、<http://localhost:4321/> を開く。
- `master` へ push すると、GitHub Actions(`.github/workflows/pages.yml`)が `site/` を Astro でビルドし、GitHub Pages へ公開する。
- PR ではビルドだけを行い、公開しない。
- 詳しくは `.kiro/steering/`(`product.md`・`tech.md`・`structure.md`・`workflow.md`)。
