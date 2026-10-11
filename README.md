# えちょのざんがい

個人ブログ「えちょのざんがい」の原稿。公開先は <https://ekicyou.github.io/>。

- サイトの原稿は `site/` にある(記事は `site/_posts/`)。ルートにあるのは開発用のファイルで、公開されない。
- `master` へ push すると、GitHub Actions(`.github/workflows/pages.yml`)が `site/` を Jekyll でビルドし、GitHub Pages へ公開する。
- PR ではビルドだけを行い、公開しない。
- 詳しくは `.kiro/steering/`(`product.md`・`tech.md`・`structure.md`・`workflow.md`)。
