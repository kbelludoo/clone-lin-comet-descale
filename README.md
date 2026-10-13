# clone-lin-comet-descale

Experimental i64-scale LIN clone of Compound III `accrualDescaleFactor` / `baseTrackingAccrued`.

**Proofs and evidence live in [kbelludoo/lin-open](https://github.com/kbelludoo/lin-open)** (`src/lin_comet_descale.lin`, `test/prove_comet_descale_external.py`, `examples/comet_descale/`). This repository is the separate LIN copy only.

## Kernel (EXPERIMENTAL, not mainnet uint256)

```
factor  = baseScale / 1e6          # 0 if baseScale < 1e6 (BadDecimals)
accrued = principal * indexDelta / trackingIndexScale / factor
```

Named contrasts (recomputed in lin-open against C11 `__int128`, limb muldiv, LIN Compiler 0 VM, Python bigint, and libtcc `roundtrip-jit`):

| Path | Result |
|---|---|
| USDC 6-dec factor | `1` — 1 USDC → `1e6` reward wei |
| WETH 18-dec factor | `1e12` — 1 WETH also → `1e6` |
| WBTC 8-dec factor | `100` — 1 WBTC → `1e6` |
| 4-dec `baseScale` | `0` |
| WETH forget-descale | `1e18` vs `1e6` (`1e12x`) |
| USDC treated as 18-dec | `0` vs `1e6` |
| GMX-style 6→18 upscale | `1e18` — opposite direction |

This is **not** `divBaseWei` tracking-index wrap, **not** `accruedInterestIndices`, **not** `presentValue`, **not** GMX `adjustForDecimals`.

## Upstream

- https://github.com/compound-finance/comet `f766f51583c23acc33b2a7824654ef2029a96804`
- `contracts/CometWithExtendedAssetList.sol` blob `3490e984d133789aa1afe2e8f7e37a11d2512396`
- `contracts/CometCore.sol` `BASE_ACCRUAL_SCALE=1e6`
- License: GPL-2.0-or-later after BUSL Change Date 2025-12-31

## Files

- `src/lin_comet_descale.lin` — LIN kernel (`cad_*`)
- `comet_descale_c11.c` — independent C11 oracle (selftest)
