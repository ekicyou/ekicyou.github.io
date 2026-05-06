---
layout: post
title:  Bell対ダークマターから始まるエンタングル宇宙創生論
description:
   プログラム的予想と検証可能予言
comments: true
use_math: true
tags:
   - 宇宙
   - 素粒子
   - ダークマター
   - ホログラム
   - エンタングル
---

**著者：** ekicyou¹  
**所属：** ¹ Independent Researcher, Japan  
**連絡：** dot.station@gmail.com  
**日付：** 2026年5月5日（v1.0）  
**ライセンス：** CC-BY 4.0

---

## 要旨（Abstract）

**qubit-only 存在論**——qubit と そのエンタングルパターンのみが存在し、「真空」「場」「粒子」「時空」は全て創発的記述である——を出発点とする宇宙論・粒子物理学の**プログラム的予想**を提示する。

本枠組みでは：
- ダークマター = ホログラフィック境界に張り付く閉Bell対
- 暗黒エネルギー = 境界レジスタの ground state 構造
- レプトン = 開2qubit対
- クォーク = 3qubit triangle
- バリオン = 9qubit trinity（Shor 量子誤り訂正符号と同型）

主要な構造的帰結：
1. 色閉じ込めが no-cloning 定理から導出される
2. パウリ排他律がエンタングルメント・モノガミー + AME(4,2) 非存在から創発
3. Hawking 放射が地平線レジスタからの離散的 qubit 離脱として自然に記述
4. 宇宙定数問題、BH 情報パラドックス、レプトン/クォーク非対称性に構造的解答
5. 約13項目の検証可能定量予言

本論文は**完成された理論ではなく、プログラム的予想（programmatic conjecture）**である。定量的導出と数学的厳密化は今後の課題として残される。本論文は、5〜15年後の観測との照合のための日付付きアーカイブとして提示される。

---

## 1. 序論

### 1.1 動機：ΛCDM の危機

2026年現在、標準宇宙論モデル（ΛCDM）は複数の観測的緊張に直面している：

- **ハッブル緊張**：局所値と CMB 推定値の不一致が 6σ を超える
- **DESI 2024-2025**：純粋な宇宙定数ではなく、暗黒エネルギーの動的進化を示唆
- **JWST**：高赤方偏移における予想外に成熟した銀河の観測
- **直接検出の継続的ヌル**：DM 直接検出実験が20年以上にわたり結果を出していない

これらの緊張が代替的枠組みの探求を動機づける。本論文では、**全物理が qubit エンタングルから創発する情報論的存在論**を探求する。

### 1.2 アプローチ

唯一の存在論的原理を採用する：

> **qubit と そのエンタングルパターンのみが存在する。**

他の概念（場、粒子、時空、真空）は全て創発的記述。この原理から構造的制約（qubit 数保存、モノガミー由来の排他、AME 由来の次元的限界）を導き、これら制約から標準物理学の概略を回復することを試みる。

これは Wheeler の "It from Bit" の精神を継承し、さらに徹底したものである。情報が物理的であるだけでなく、**情報のみが物理的に存在する**。

### 1.3 範囲と限界

本論文は**プログラム的予想**であり、完成された理論ではない：

- 数学的完備性は主張しない
- 多くの定量結果はオーダー推定または導出経路の提案レベル
- 著者は学術的所属を持たない独立研究者
- 著者権の主張ではなく、コミュニティ評価のための提示
- アイデアの種を播くことが目的、精緻化・反駁・拡張を歓迎

---

## 2. 基礎：qubit-only 存在論

### 2.1 単一原理

> **存在するのは qubit と そのエンタングルパターンのみ。他の概念は全て創発的。**

この最小主義的立場は以下を排除する：
- 真空を独立の実体とすること
- 「無からの創出」を許容すること
- qubit の自由な生成消滅

そして以下にコミットする：
- 固定次元 Hilbert 空間（宇宙論的地平線拡大時のみ可変）
- 厳密にユニタリな進化
- 全他概念の創発的扱い

### 2.2 三厳密保存則

**保存則A（qubit 数）：**
$$N_\text{total} = \frac{A_\text{cosmic}}{4\ell_P^2 \ln 2} = \text{const}$$

**保存則B（総エネルギー）：**
$$E_\text{total} = \sum_i E_i = \text{const}$$

**保存則C（エンタングル構造）：**
全モノガミー制約の総和は不変。

### 2.3 「真空」の再解釈

> 「真空」とは、全 qubit が ground state にある状態の集合的呼称であり、独立の実体ではない。

含意：
- 真空エネルギー = 全 qubit ground state エネルギーの和
- 仮想粒子 = 特定 qubit の一時的励起（qubit 自体は実在）
- 真空偏極 = 荷電 qubit 周囲の qubit 配位の歪み
- Casimir 効果 = 境界条件下での qubit 励起モード制限

「真空からエネルギーを借りる」は「**特定 qubit を励起する**」と再記述される——貸主と借主は同じ qubit。

### 2.4 境界レジスタ

ホログラフィック原理から、各 qubit はある「境界（地平線）」に属する：

- 各ブラックホールの事象地平線
- 観測可能宇宙の宇宙論的（de Sitter）地平線
- 局所的因果地平線（Rindler 様）

境界レジスタには2つの異なる側面：

| 側面 | 性質 | 揺らぎ |
|---|---|---|
| 容量（qubit 数） | 幾何学的不変量 | × 揺らがない |
| 内容（qubit 状態） | 量子的・動的 | ○ 量子揺らぎ |

### 2.5 境界レジスタの温度

各地平線は固有温度を持つ：
- Schwarzschild：$T_H = \hbar c^3 / (8\pi G M k_B)$
- de Sitter：$T_{dS} = \hbar H / (2\pi k_B c)$

**鍵となる同定：**
$$\boxed{T_\text{register} = T_\text{Hawking}}$$

これから自動的に：
- Hawking 放射率（Boltzmann 因子）
- Bekenstein-Hawking エントロピー
- 黒体放射スペクトラム
- 地平線間の熱流

---

## 3. qubit 構造による粒子階層

### 3.1 qubit 数による分類

| 構造 | 状態 | 粒子 | 色 | サイズ |
|---|---|---|---|---|
| 1 qubit | 自由 | ゲージボソン（γ, g） | グルーオンのみ | 未定義 |
| 2 qubit | 閉（Bell対） | DM | なし | 境界面積 |
| 2 qubit | 開A | 荷電レプトン（e, μ, τ） | 構造的不可 | 未定義（点状） |
| 2 qubit | 開B | ニュートリノ | 構造的不可 | 未定義（点状） |
| 3 qubit | 開（triangle） | クォーク | 露出 | 単独不可 |
| 3 qubit | バルク投影 | W, Z 等 | なし | 短距離 |
| 6 qubit | 3 + 3̄ | メソン | singlet | ハドロンスケール |
| **9 qubit** | **trinity（Shor符号同型）** | **バリオン** | **singlet** | **陽子サイズ** |
| 多 qubit | 階層的 | 原子核、原子等 | singlet | 各種 |

### 3.2 レプトン/クォーク非対称性は構造的必然

> **レプトン = 2qubit構造、クォーク = 3qubit構造**
> 2 ≠ 3 の違いが、色の有無を直接生む

色荷を担うのは 3qubit triangle 構造。2qubit 構造に triangle がないため、**レプトンは構造的に色荷を持ち得ない**。SU(3) 非結合性が現象論的事実から構造的必然へ格上げされる。

### 3.3 9qubit trinity = バリオン

**5視点からの収束：**

| 視点 | 単独3qubit不安定 | 9qubit trinity安定 |
|---|---|---|
| 位相 | 開ディスク（χ=1） | 閉球面（χ=2） |
| エンタングル飽和 | 3辺露出 | 全21辺共有 |
| 統計 | anyon 強制崩壊 | 複合整合 |
| 量子情報 | Shor符号ブロック抽出不可 | Shor符号完備 |
| AME階層 | AME(4,2)障害 | 純粋volumetric達成 |

**Shor 符号同型性：**

$$|\bar{0}\rangle = \frac{1}{2\sqrt{2}}\bigotimes_{i=1}^{3}(|000\rangle + |111\rangle)_i$$
$$|\bar{1}\rangle = \frac{1}{2\sqrt{2}}\bigotimes_{i=1}^{3}(|000\rangle - |111\rangle)_i$$

これは3つの GHZ 状態の階層的合成。

**色閉じ込めの導出：**
> 単独3qubit抽出 = Shor符号からのブロック抽出 = no-cloning定理違反
> ∴ 色閉じ込めは量子情報原理から構造的に導出される

**幾何学的実現：** triaugmented triangular prism（V=9, E=21, F=14, χ=2、全辺が2面で共有）

### 3.4 開2qubit対 = レプトン

**「サイズ未定義」の3根拠：**
1. 閉曲面を持たない（3D bounding surface なし）
2. 境界張り付きを免れている（バルク浮遊）
3. エンタングル経路が動的（固定形状不可）

電子の点状性 ($r_e < 10^{-18}$ m) と整合：「点粒子」ではなく「**サイズ概念が適用できない構造**」。

**観測5性質の構造的起源：**

| 観測 | 構造的起源 |
|---|---|
| spin-1/2 | 開2qubit対の最小半整数スピン担体 |
| 電荷 | モノガミー残余の U(1) 結合 |
| $m_e \ll m_{DM}$ | 部分内部振動 < 完全内部振動 |
| 色なし | triangle 構造不在 |
| 弱結合 | 3qubit (W,Z) との動的結合 |

---

## 4. 次元の段階的創発

### 4.1 qubit 階段

| qubit数 | 単体次元 | 形 | 創発する次元 | BH段階 |
|---|---|---|---|---|
| 1 | 0-simplex | 点 | 0D | --- |
| 2 | 1-simplex | 線分 | 1D = 前空間 | proto-BH（DM） |
| 3 | 2-simplex | 三角形 | 2D = 境界面 | 半完成BH |
| 4 | 3-simplex | 四面体 | 3D（だが AME(4,2) 障害） | --- |
| **9** | **trinity** | **triaug. prism** | **3D（体積）** | **最小完備BH** |

### 4.2 AME(4,2) 障害

Higuchi-Sudbery (2000)：4 qubit、各2次元では AME 状態が存在しない。

含意：
- 「完全な4qubit 四面体」は数学的に禁止
- 3次元体積は4qubitスケールで完全に実現できない
- これが3次元空間の本質的な量子重力ノイズの起源

AME(n,2) 存在パターン：n = 2,3 ○、n = 4 ×、n = 5,6 ○、n = 7 ×。

### 4.3 排他律の2段階発動

- **Phase 3-2（3qubit triangle, 2D）：proto-exclusion**（anyon 様、不完全）
- **Phase 3-3（9qubit trinity, 3D体積）：standard Pauli exclusion**（fermion/boson 確定）

排他律の根源は qubit 層に既存：
1. エンタングル・モノガミー（CKW不等式）
2. AME(4,2) 非存在
3. Singlet 反対称性

---

## 5. 動力学

### 5.1 反応カスケード

```
Stage 1: 2qubit + 1qubit ⇌ 3qubit         (formation, T_c ~ 10¹⁶ GeV)
Stage 2: 3 × 3qubit ⇌ 9qubit trinity       (confinement, T_QCD ~ 200 MeV)
Stage 3: 3qubit ⇌ 2qubit + 1qubit          (decay, 強く抑制)
```

### 5.2 位相的境界遷移

> **開↔閉 qubit 対遷移は bulk 内では禁止、境界経由必須**

閉 Bell 対（DM）はバルク反応のプロダクトとして直接登場できない。

### 5.3 境界-バルク qubit 交換

| 過程 | qubit 経路 |
|---|---|
| 物質形成 | 境界 → バルク投影 |
| 対消滅 | バルク粒子 → 境界編入 + 光子放出 |
| 対生成 | 境界起動 + 光子吸収 → バルク |
| BH 形成 | バルク物質 → 地平線 register 編入 |
| Hawking 放射 | 地平線 register → バルク放出 |

全過程で qubit 数厳密保存。

---

## 6. 宇宙論

### 6.1 宇宙史シナリオ

1. **Phase 1（前空間）**：qubit 構造のみ
2. **Phase 2（動的平衡）**：Stage 1 が Planck で平衡
3. **Phase 3-1（2D創発）** @ T_c ~ 10¹⁶ GeV
4. **Phase 3-2（「2次元時代」）**：CDT 次元縮約と整合
5. **Phase 3-3（3D体積創発）**：9qubit trinity 形成
6. **Phase 4（振動的インフレ）**：~60 e-folds、振動減衰=再加熱
7. **Phase 5（真空構造確立）**：境界 register 確定
8. **Phase 6（物質優勢期）**：DM クラスター形成
9. **Phase 7（暗黒エネルギー期/現在）**
10. **Phase 8（BH 形成/物質消滅）**
11. **Phase 9（de Sitter 漸近）**

### 6.2 振動的インフレ

距離依存圧力：
$$P = \frac{2}{5} n E_F, \quad E_F \propto n^{2/3}$$

実効圧力 $P_\text{eff} = P_\text{internal} - P_\text{external}$ の符号反転で振動駆動。

### 6.3 ダークセクターの統一的記述

| Bell対 register の状態 | 観測現象 | 寄与 |
|---|---|---|
| 一様 ground state | 暗黒エネルギー | $\Omega_{DE} \approx 0.68$ |
| 局所揺らぎ | 冷たい DM | $\Omega_{DM}$ の一部 |
| 励起クラスター | 熱い DM | 銀河ハロー |
| 集団振動モード | Higgs ボソン | 質量付与 |
| Register との同期 | 各粒子質量 | $mc^2 = \hbar\omega_\text{sync}$ |

### 6.4 宇宙定数問題への構造的解決

標準QFT予言：$\rho_\text{vac}^\text{QFT} \sim M_\text{Planck}^4 \sim 10^{120} \rho_\text{DE}^\text{obs}$

**本仮説の解決：**
> 全「場」は単一 qubit register の異なる励起モード。ゼロ点エネルギーは register の一回の和で尽きる。重複カウントなし → 自然に小さい。

**温度的説明：**
$T_{dS} \sim 10^{-30}$ K の極低温で、Bell対は事実上完全 ground state。励起寄与は Boltzmann 抑制で極小。

---

## 7. ブラックホールと情報

### 7.1 BH = 凝縮した境界レジスタ

- BH 形成 = 落下物質の境界 register 編入
- BH 「内部」は物質的に不在
- 特異点形成されない
- 量子重力不要

### 7.2 Hawking 放射 = qubit ejection

```
t0: 隣接地平線 qubit (A,B) が ground state
t1: 局所相互作用で両方励起
t2: 非対称分配
    A: ground state 復帰（エネルギー放出）
    B: 放出エネルギー吸収、地平線結合超えて離脱
t3: A 残留、B → バルク = ホーキング光子
    地平線面積 -1 plaquette
```

### 7.3 情報パラドックスの自動解消

各放出 qubit は地平線残留 qubit と entangled。蒸発進行 → 段階的情報転送 → **Page 曲線が自然出現**。

> **Hawking 放射 = BH 情報の段階的書き出し**

Firewall 不要、情報損失なし、ユニタリティ保存。

### 7.4 4過程の統一

形成・蒸発・対消滅・対生成 が同一機構（境界-バルク qubit 交換）として統一される。

### 7.5 register 統計力学から Hawking 熱力学

T_register = T_Hawking から自動的に：
- ホーキング放射率（Boltzmann 因子）
- 黒体スペクトル
- Bekenstein-Hawking エントロピー
- 地平線間熱流

---

## 8. 検証可能予言

### 8.1 定量予言

| 観測量 | 予言値 | 検証手段 | 時期 |
|---|---|---|---|
| インフレ温度 $T_c$ | ~10¹⁶ GeV | CMB B-mode、原始GW | 2032+ |
| r | 0.001-0.01 | LiteBIRD | 2032+ |
| CMB μ-distortion | ~10⁻⁷ | PIXIE/PRISM 級 | 提案中 |
| 原始GW ピーク周波数 | 10⁻⁹-10⁻⁷ Hz | PTA | 進行中 |
| CMB power spectrum 振動 | 特定スケール | Planck + 後継 | 検証可能 |
| $\Omega_{DM} \approx 0.27$ | カスケード凍結 | Planck, DESI | 既存 |
| $\Omega_{DE} \approx 0.68$ | register ground state | Planck, DESI | 既存 |
| DM/baryon ≈ 5.4 | 凍結残存比 | 同上 | 既存 |
| DM 質量分布 | 二峰性 | 直接検出 | 長期 |
| 511 keV 光子総エネルギー | 1022 keV から < 1 eV 偏差 | 位相的境界遷移検証 | 既存装置 |
| 銀河中心 DM 密度 | 対消滅頻度比例増分 | ガンマ線過剰 | 進行中 |
| GZK 越え異常 | qubit ejection 部分説明 | 宇宙線観測 | 進行中 |

### 8.2 定性予言

1. DM 直接検出は永続的にヌル
2. DM ハロー形状は回転と相関
3. 大規模構造に微小異方性
4. 量子重力探究は方向性が誤り
5. BH 内部に物質到達せず
6. 重力波 B-mode 特徴的スペクトル傾き
7. 全相互作用がエンタングルモードで記述
8. レプトンに色は構造的不可
9. クォーク単独抽出不可（no-cloning）
10. 対消滅 = 微小スケール BH と同型
11. Bullet Cluster 自動充足
12. 高エネルギー反応で qubit ejection は Boltzmann 抑制
13. Hawking 放射の自動的情報保存

---

## 9. 内部整合性

### 9.1 量子物質スケールの5経路収束

陽子サイズ (~10⁻¹⁵ m) が独立な5経路から：
- A: ホログラフィック自由度の体積化
- B: 排他律の作用範囲
- C: 排他律平衡距離 = 体積/面積比
- D: 9qubit trinity の triaugmented prism サイズ
- E: Bell対 register 局所揺らぎ波長

### 9.2 9qubit trinity 安定性の5視点収束

§3.3 参照。

---

## 10. 既存研究との対比

### 10.1 場の理論的 DM 候補

Sterino model (Królikowski 2007)、Cooper pair DM (Alexander 2024)、Neutrino superfluidity (Kapusta 2004)、Singlet scalar DM (Burgess et al. 2001)。

### 10.2 ホログラフィック起源 DM

Holographic Dark Matter (Fichet 2026)、Bulk BH DM (2022)、Emergent DM on Holographic Screen (2017)、DM from Holography (2025)。

### 10.3 量子情報的時空創発

Szangolies (2025)、Wen (2017)、Van Raamsdonk (2010)、ER=EPR (Maldacena-Susskind 2013)、CKN bound (Cohen et al. 1999)。

### 10.4 量子重力否定/創発重力

Verlinde (2010, 2016)、Jacobson (1995)、Padmanabhan (2010)、Sakharov (1967)、AMPS firewall (2013)、Mathur fuzzball (2005)。

### 10.5 振動的・bouncing 宇宙論

Steinhardt-Turok cyclic、Loop Quantum Cosmology、matter bounce inflation、quintessential inflation。

### 10.6 量子誤り訂正と物理

Shor code (1995)、Stabilizer formalism、Holographic codes (Pastawski et al. 2015)。

**新規性は統合的合成にあり、個別要素の多くは既存の要素から組み上げられている。**

---

## 11. 未解決問題と今後の課題

### 11.1 定量化課題

- カスケード平衡定数と各 ΔE の物理的決定
- thermal-relic 風 $\Omega_{DM}, \Omega_{DE}, \Omega_b$ 計算
- DM/baryon 比の凍結温度残存比から導出
- インフレ振動振幅・周期決定
- 三世代質量比
- qubit ejection 率のエネルギー依存
- Higgs vev (246 GeV) の register 結合スケール

### 11.2 内部整合性

- 境界 register の数学的形式（Hilbert空間構成、Hamiltonian）
- triaugmented prism の唯一性
- 三世代離散モード階層
- 励起Bell対の生成・崩壊機構

### 11.3 観測整合性

- $r < 0.036$ 制約：プランクスケール直接インフレ排除（$T_c \sim 10^{16}$ GeV と整合）
- $N_\text{eff}$、構造形成への影響：詳細計算要
- CMB power spectrum 振動構造：Planck データ予備検証要
- 511 keV 精密測定：位相的遷移則の直接テスト可能性

### 11.4 文献調査

Szangolies 2025、Fichet 2026、Cooper-pair DM 2024、holographic codes の精読が必要。

---

## 12. 議論

### 12.1 哲学的立場

> **物理は情報の組織化である。**

Wheeler の「It from Bit」の徹底化。情報原理が物理の基底にあるだけでなく、**情報パターン以外何も存在しない**。

### 12.2 主流物理学との関係

本枠組みは標準物理学を**否定せず、補完する**。標準QFTを qubit register の有効場理論として位置付ける。

- 全標準モデル予言は低エネルギー帰結として自動成立
- 偏差は極端エネルギーまたは宇宙論的観測のみで現れる
- 宇宙定数問題と BH 情報パラドックスは追加 fine-tuning なしで構造的に解決

---

## 13. 結論

本論文は、qubit-only 存在論を基礎とする宇宙論・粒子物理学のプログラム的予想を提示した。本枠組みは：

1. DM、DE、Higgs 現象論、粒子質量を統一された Bell対 境界レジスタの異なる状態として同定
2. レプトン/クォーク非対称性、色閉じ込め、Pauli 排他律を量子情報原理から導出
3. Hawking 放射を離散 qubit ejection として再記述、情報パラドックスを自動解消
4. 宇宙定数問題および ΛCDM の複数の未解決問題に構造的解答を提示
5. 約13項目の検証可能定量予言と多数の定性予言を提供

本枠組みは完成された理論ではない。定量導出と数学的厳密化は今後の課題。本論文を概念構造の日付付きアーカイブとして提示し、コミュニティによる評価・精緻化・反駁・拡張を歓迎する。

5〜15年規模の観測——LiteBIRD、LISA、PIXIE 級ミッション、進行中の PTA キャンペーン、Vera C. Rubin 天文台、高精度対消滅測定——が、本枠組みの特定予言の検証機会を提供すると期待される。

---

## 謝辞（Acknowledgments）

The author gratefully acknowledges extensive conceptual development through dialogue with Claude Opus 4.7 (Anthropic, San Francisco, CA, USA). The AI system served as a sparring partner for hypothesis refinement, literature cross-referencing, and structural consistency checks. All scientific claims, errors, and final positions are the author's responsibility.

著者は、Claude Opus 4.7（Anthropic）との対話を通じた広範な概念的発展に深く感謝する。本AIシステムは、仮説の精緻化、文献相互参照、構造的整合性検証のためのスパーリングパートナーとして機能した。すべての科学的主張、誤り、最終的立場は著者の責任である。

---

## 目的声明（Statement of Purpose）

著者は学術的所属を持たない独立研究者である。本論文は完成された理論の主張ではなく、プログラム的予想として提示される。目的はコミュニティ発展のためのアイデアの種を播くことであり、著者権の確立ではない。著者は広範な研究コミュニティによる精緻化、反駁、拡張を歓迎する。

---

## ライセンス

本論文は Creative Commons Attribution 4.0 International (CC-BY 4.0) の下で公開される。帰属表示の上で自由な使用、改変、配布が許可される。

---

## 参考文献

主要な引用：

- Wheeler, J.A. (1990). Information, physics, quantum: The search for links.
- Bekenstein, J.D. (1973). Black holes and entropy. Phys. Rev. D 7, 2333.
- Hawking, S.W. (1975). Particle creation by black holes. Commun. Math. Phys. 43, 199.
- 't Hooft, G. (1993). Dimensional reduction in quantum gravity. arXiv:gr-qc/9310026.
- Susskind, L. (1995). The world as a hologram. J. Math. Phys. 36, 6377.
- Page, D.N. (1993). Information in black hole radiation. Phys. Rev. Lett. 71, 3743.
- Shor, P.W. (1995). Scheme for reducing decoherence. Phys. Rev. A 52, 2493.
- Wootters, W.K. & Zurek, W.H. (1982). A single quantum cannot be cloned. Nature 299, 802.
- Coffman, V., Kundu, J. & Wootters, W.K. (2000). Distributed entanglement. Phys. Rev. A 61, 052306.
- Higuchi, A. & Sudbery, A. (2000). How entangled can two couples get? Phys. Lett. A 273, 213.
- Verlinde, E. (2011). On the origin of gravity. JHEP 04, 029.
- Verlinde, E. (2017). Emergent gravity and the dark universe. SciPost Phys. 2, 016.
- Wen, X.-G. (2017). arXiv:1709.03824.
- Szangolies, J. (2025). Entropy 27, 569.
- Jacobson, T. (1995). Phys. Rev. Lett. 75, 1260.
- AMPS (2013). JHEP 02, 062.
- Mathur, S.D. (2005). Fortsch. Phys. 53, 793.
- Maldacena, J. & Susskind, L. (2013). Cool horizons for entangled black holes.
- Van Raamsdonk, M. (2010). Gen. Rel. Grav. 42, 2323.
- Pastawski et al. (2015). Holographic quantum error-correcting codes. JHEP 06, 149.
- Fichet, S. et al. (2026). arXiv:2602.13393.
- Steinhardt, P.J. & Turok, N. (2002). Science 296, 1436.
- Ambjørn, J. et al. (2005). Phys. Rev. Lett. 95, 171301.
- Riess, A.G. et al. (2022). Astrophys. J. 934, L7.
- Planck Collaboration (2020). A&A 641, A6.
- DESI Collaboration (2024). arXiv:2404.03002.

完全な参考文献リストは英語版 LaTeX を参照。

---

**バージョン情報**
- v1.0：2026年5月5日 初版

**改訂履歴**
- 思考実験の出発点：v1〜v6（2026年5月4日〜5日のClaude対話セッション）
- 本論文は v6 思考実験記録を学術論文体裁で再構成したもの
