﻿std::vector<std::string> testMove
{
	// NO.1
	"h00 y00 e00 g00 k00 u00 r00\n"
	"y_e_g_k_o_k_g_e_y_\n"
	" .r_ . . . . .u_ .\n"
	"h_h_h_h_h_h_h_h_h_\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	"^h^h^h^h^h^h^h^h^h\n"
	" .^u . . . . .^r .\n"
	"^y^e^g^k^o^k^g^e^y\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"first"
	":"
	"９六 歩(9,7)\n"
	"８六 歩(8,7)\n"
	"７六 歩(7,7)\n"
	"６六 歩(6,7)\n"
	"５六 歩(5,7)\n"
	"４六 歩(4,7)\n"
	"３六 歩(3,7)\n"
	"２六 歩(2,7)\n"
	"１六 歩(1,7)\n"
	"１八 飛(2,8)\n"
	"３八 飛(2,8)\n"
	"４八 飛(2,8)\n"
	"５八 飛(2,8)\n"
	"６八 飛(2,8)\n"
	"７八 飛(2,8)\n"
	"９八 杏(9,9)\n"
	"７八 銀(7,9)\n"
	"６八 銀(7,9)\n"
	"７八 金(6,9)\n"
	"６八 金(6,9)\n"
	"５八 金(6,9)\n"
	"６八 玉(5,9)\n"
	"５八 玉(5,9)\n"
	"４八 玉(5,9)\n"
	"５八 金(4,9)\n"
	"４八 金(4,9)\n"
	"３八 金(4,9)\n"
	"４八 銀(3,9)\n"
	"３八 銀(3,9)\n"
	"１八 杏(1,9)\n"
	,
	// NO.2
	"h00 y00 e00 g00 k00 u00 r00\n"
	" . . . . . . . . .\n"
	"^h . . . . . . . .\n"
	" .^h . . . . . . .\n"
	" . .^h . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"first"
	":"
	"９一 歩(9,2) 成り\n"
	"８二 歩(8,3)\n"
	"８二 歩(8,3) 成り\n"
	"７三 歩(7,4)\n"
	"７三 歩(7,4) 成り\n"
	,
	// NO.3
	"h00 y00 e00 g00 k00 u00 r00\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . .h_ . .\n"
	" . . . . . . .h_ .\n"
	" . . . . . . . .h_\n"
	" . . . . . . . . .\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"second"
	":"
	"３七 歩(3,6)\n"
	"３七 歩(3,6) 成り\n"
	"２八 歩(2,7)\n"
	"２八 歩(2,7) 成り\n"
	"１九 歩(1,8) 成り\n"
	,
	// NO.4
	"h00 y00 e00 g00 k00 u00 r00\n"
	" . . . . . . . . .\n"
	"^y . . . . . . . .\n"
	" .^y . . . . . . .\n"
	" . .^y . . . . . .\n"
	" . . .^y . . . . .\n"
	" . . . .^y . . . .\n"
	"^hh_ . . .^y . . .\n"
	" . . . . . .^y . .\n"
	"^y^y . . . . .^y .\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"first"
	":"
	"９一 杏(9,2) 成り\n"
	"８二 杏(8,3)\n"
	"８二 杏(8,3) 成り\n"
	"８一 杏(8,3) 成り\n"
	"７三 杏(7,4)\n"
	"７三 杏(7,4) 成り\n"
	"７二 杏(7,4)\n"
	"７二 杏(7,4) 成り\n"
	"７一 杏(7,4) 成り\n"
	"６四 杏(6,5)\n"
	"６三 杏(6,5)\n"
	"６三 杏(6,5) 成り\n"
	"６二 杏(6,5)\n"
	"６二 杏(6,5) 成り\n"
	"６一 杏(6,5) 成り\n"
	"５五 杏(5,6)\n"
	"５四 杏(5,6)\n"
	"５三 杏(5,6)\n"
	"５三 杏(5,6) 成り\n"
	"５二 杏(5,6)\n"
	"５二 杏(5,6) 成り\n"
	"５一 杏(5,6) 成り\n"
	"９六 歩(9,7)\n"
	"４六 杏(4,7)\n"
	"４五 杏(4,7)\n"
	"４四 杏(4,7)\n"
	"４三 杏(4,7)\n"
	"４三 杏(4,7) 成り\n"
	"４二 杏(4,7)\n"
	"４二 杏(4,7) 成り\n"
	"４一 杏(4,7) 成り\n"
	"３七 杏(3,8)\n"
	"３六 杏(3,8)\n"
	"３五 杏(3,8)\n"
	"３四 杏(3,8)\n"
	"３三 杏(3,8)\n"
	"３三 杏(3,8) 成り\n"
	"３二 杏(3,8)\n"
	"３二 杏(3,8) 成り\n"
	"３一 杏(3,8) 成り\n"
	"９八 杏(9,9)\n"
	"８八 杏(8,9)\n"
	"８七 杏(8,9)\n"
	"２八 杏(2,9)\n"
	"２七 杏(2,9)\n"
	"２六 杏(2,9)\n"
	"２五 杏(2,9)\n"
	"２四 杏(2,9)\n"
	"２三 杏(2,9)\n"
	"２三 杏(2,9) 成り\n"
	"２二 杏(2,9)\n"
	"２二 杏(2,9) 成り\n"
	"２一 杏(2,9) 成り\n"
	,
	// NO.5
	"h00 y00 e00 g00 k00 u00 r00\n"
	"y_y_ . . . . . . .\n"
	" . . . . . . . .y_\n"
	" . . . . . . .y_ .\n"
	"^hh_ . . . .y_ . .\n"
	" . . . . .y_ . . .\n"
	" . . . .y_ . . . .\n"
	" . . .y_ . . . . .\n"
	" . .y_ . . . . . .\n"
	" .y_ . . . . . . .\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"second"
	":"
	"９二 杏(9,1)\n"
	"９三 杏(9,1)\n"
	"９四 杏(9,1)\n"
	"８二 杏(8,1)\n"
	"８三 杏(8,1)\n"
	"１三 杏(1,2)\n"
	"１四 杏(1,2)\n"
	"１五 杏(1,2)\n"
	"１六 杏(1,2)\n"
	"１七 杏(1,2)\n"
	"１七 杏(1,2) 成り\n"
	"１八 杏(1,2)\n"
	"１八 杏(1,2) 成り\n"
	"１九 杏(1,2) 成り\n"
	"２四 杏(2,3)\n"
	"２五 杏(2,3)\n"
	"２六 杏(2,3)\n"
	"２七 杏(2,3)\n"
	"２七 杏(2,3) 成り\n"
	"２八 杏(2,3)\n"
	"２八 杏(2,3) 成り\n"
	"２九 杏(2,3) 成り\n"
	"８五 歩(8,4)\n"
	"３五 杏(3,4)\n"
	"３六 杏(3,4)\n"
	"３七 杏(3,4)\n"
	"３七 杏(3,4) 成り\n"
	"３八 杏(3,4)\n"
	"３八 杏(3,4) 成り\n"
	"３九 杏(3,4) 成り\n"
	"４六 杏(4,5)\n"
	"４七 杏(4,5)\n"
	"４七 杏(4,5) 成り\n"
	"４八 杏(4,5)\n"
	"４八 杏(4,5) 成り\n"
	"４九 杏(4,5) 成り\n"
	"５七 杏(5,6)\n"
	"５七 杏(5,6) 成り\n"
	"５八 杏(5,6)\n"
	"５八 杏(5,6) 成り\n"
	"５九 杏(5,6) 成り\n"
	"６八 杏(6,7)\n"
	"６八 杏(6,7) 成り\n"
	"６九 杏(6,7) 成り\n"
	"７九 杏(7,8) 成り\n"
	,
	// NO.6
	"h00 y00 e00 g00 k00 u00 r00\n"
	" . . . . . . . . .\n"
	" . . .^h .h_ . . .\n"
	"^e .^e . . . . .^e\n"
	" .^e . .^e . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"first"
	":"
	"６一 歩(6,2) 成り\n"
	"８五 桂(7,3)\n"
	"２五 桂(1,3)\n"
	"９六 桂(8,4)\n"
	"６六 桂(5,4)\n"
	,
	// NO.7
	"h00 y00 e00 g00 k00 u00 r00\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . .e_ . .e_ .\n"
	"e_ . . . . .e_ .e_\n"
	" . . .^h .h_ . . .\n"
	" . . . . . . . . .\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"second"
	":"
	"６四 桂(5,6)\n"
	"３四 桂(2,6)\n"
	"４五 桂(3,7)\n"
	"２五 桂(1,7)\n"
	"４九 歩(4,8) 成り\n"
	,
	// NO.8
	"h00 y00 e00 g00 k00 u00 r00\n"
	"^g . . . . . . .^g\n"
	" .^g . . . . .^g .\n"
	" . . . .^g . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . .^g . . . .\n"
	" .^g . . . . .^g .\n"
	"^g . . . . . . .^g\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"first"
	":"
	"８一 銀(8,2)\n"
	"７一 銀(8,2)\n"
	"９三 銀(8,2)\n"
	"７三 銀(8,2)\n"
	"８一 銀(8,2) 成り\n"
	"７一 銀(8,2) 成り\n"
	"９三 銀(8,2) 成り\n"
	"７三 銀(8,2) 成り\n"
	"３一 銀(2,2)\n"
	"２一 銀(2,2)\n"
	"３三 銀(2,2)\n"
	"１三 銀(2,2)\n"
	"３一 銀(2,2) 成り\n"
	"２一 銀(2,2) 成り\n"
	"３三 銀(2,2) 成り\n"
	"１三 銀(2,2) 成り\n"
	"６二 銀(5,3)\n"
	"５二 銀(5,3)\n"
	"４二 銀(5,3)\n"
	"６四 銀(5,3)\n"
	"４四 銀(5,3)\n"
	"６二 銀(5,3) 成り\n"
	"５二 銀(5,3) 成り\n"
	"４二 銀(5,3) 成り\n"
	"６四 銀(5,3) 成り\n"
	"４四 銀(5,3) 成り\n"
	"６六 銀(5,7)\n"
	"５六 銀(5,7)\n"
	"４六 銀(5,7)\n"
	"６八 銀(5,7)\n"
	"４八 銀(5,7)\n"
	"９七 銀(8,8)\n"
	"８七 銀(8,8)\n"
	"７七 銀(8,8)\n"
	"７九 銀(8,8)\n"
	"３七 銀(2,8)\n"
	"２七 銀(2,8)\n"
	"１七 銀(2,8)\n"
	"３九 銀(2,8)\n"
	"９八 銀(9,9)\n"
	"１八 銀(1,9)\n"
	,
	// NO.9
	"h00 y00 e00 g00 k00 u00 r00\n"
	"g_ . . . . . . .g_\n"
	" .g_ . . . . .g_ .\n"
	" . . . .g_ . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . .g_ . . . .\n"
	" .g_ . . . . .g_ .\n"
	"g_ . . . . . . .g_\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"second"
	":"
	"９二 銀(9,1)\n"
	"１二 銀(1,1)\n"
	"９三 銀(8,2)\n"
	"８三 銀(8,2)\n"
	"７三 銀(8,2)\n"
	"７一 銀(8,2)\n"
	"３三 銀(2,2)\n"
	"２三 銀(2,2)\n"
	"１三 銀(2,2)\n"
	"３一 銀(2,2)\n"
	"６四 銀(5,3)\n"
	"５四 銀(5,3)\n"
	"４四 銀(5,3)\n"
	"６二 銀(5,3)\n"
	"４二 銀(5,3)\n"
	"６八 銀(5,7)\n"
	"５八 銀(5,7)\n"
	"４八 銀(5,7)\n"
	"６六 銀(5,7)\n"
	"４六 銀(5,7)\n"
	"６八 銀(5,7) 成り\n"
	"５八 銀(5,7) 成り\n"
	"４八 銀(5,7) 成り\n"
	"６六 銀(5,7) 成り\n"
	"４六 銀(5,7) 成り\n"
	"８九 銀(8,8)\n"
	"７九 銀(8,8)\n"
	"９七 銀(8,8)\n"
	"７七 銀(8,8)\n"
	"８九 銀(8,8) 成り\n"
	"７九 銀(8,8) 成り\n"
	"９七 銀(8,8) 成り\n"
	"７七 銀(8,8) 成り\n"
	"３九 銀(2,8)\n"
	"２九 銀(2,8)\n"
	"３七 銀(2,8)\n"
	"１七 銀(2,8)\n"
	"３九 銀(2,8) 成り\n"
	"２九 銀(2,8) 成り\n"
	"３七 銀(2,8) 成り\n"
	"１七 銀(2,8) 成り\n"
	,
	// NO.10
	"h00 y00 e00 g00 k00 u00 r00\n"
	"^G . . . . . . .^G\n"
	" .^G . . . . .^G .\n"
	" . . . .^G . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . .^G . . . .\n"
	" .^G . . . . .^G .\n"
	"^G . . . . . . .^G\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"first"
	":"
	"８一 全(9,1)\n"
	"９二 全(9,1)\n"
	"２一 全(1,1)\n"
	"１二 全(1,1)\n"
	"８一 全(8,2)\n"
	"７一 全(8,2)\n"
	"９二 全(8,2)\n"
	"７二 全(8,2)\n"
	"８三 全(8,2)\n"
	"３一 全(2,2)\n"
	"２一 全(2,2)\n"
	"３二 全(2,2)\n"
	"１二 全(2,2)\n"
	"２三 全(2,2)\n"
	"６二 全(5,3)\n"
	"５二 全(5,3)\n"
	"４二 全(5,3)\n"
	"６三 全(5,3)\n"
	"４三 全(5,3)\n"
	"５四 全(5,3)\n"
	"６六 全(5,7)\n"
	"５六 全(5,7)\n"
	"４六 全(5,7)\n"
	"６七 全(5,7)\n"
	"４七 全(5,7)\n"
	"５八 全(5,7)\n"
	"９七 全(8,8)\n"
	"８七 全(8,8)\n"
	"７七 全(8,8)\n"
	"９八 全(8,8)\n"
	"７八 全(8,8)\n"
	"８九 全(8,8)\n"
	"３七 全(2,8)\n"
	"２七 全(2,8)\n"
	"１七 全(2,8)\n"
	"３八 全(2,8)\n"
	"１八 全(2,8)\n"
	"２九 全(2,8)\n"
	"９八 全(9,9)\n"
	"８九 全(9,9)\n"
	"１八 全(1,9)\n"
	"２九 全(1,9)\n"
	,
	// NO.11
	"h00 y00 e00 g00 k00 u00 r00\n"
	"G_ . . . . . . .G_\n"
	" .G_ . . . . .G_ .\n"
	" . . . .G_ . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . .G_ . . . .\n"
	" .G_ . . . . .G_ .\n"
	"G_ . . . . . . .G_\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"second"
	":"
	"９二 全(9,1)\n"
	"８一 全(9,1)\n"
	"１二 全(1,1)\n"
	"２一 全(1,1)\n"
	"９三 全(8,2)\n"
	"８三 全(8,2)\n"
	"７三 全(8,2)\n"
	"９二 全(8,2)\n"
	"７二 全(8,2)\n"
	"８一 全(8,2)\n"
	"３三 全(2,2)\n"
	"２三 全(2,2)\n"
	"１三 全(2,2)\n"
	"３二 全(2,2)\n"
	"１二 全(2,2)\n"
	"２一 全(2,2)\n"
	"６四 全(5,3)\n"
	"５四 全(5,3)\n"
	"４四 全(5,3)\n"
	"６三 全(5,3)\n"
	"４三 全(5,3)\n"
	"５二 全(5,3)\n"
	"６八 全(5,7)\n"
	"５八 全(5,7)\n"
	"４八 全(5,7)\n"
	"６七 全(5,7)\n"
	"４七 全(5,7)\n"
	"５六 全(5,7)\n"
	"８九 全(8,8)\n"
	"７九 全(8,8)\n"
	"９八 全(8,8)\n"
	"７八 全(8,8)\n"
	"８七 全(8,8)\n"
	"３九 全(2,8)\n"
	"２九 全(2,8)\n"
	"３八 全(2,8)\n"
	"１八 全(2,8)\n"
	"２七 全(2,8)\n"
	"８九 全(9,9)\n"
	"９八 全(9,9)\n"
	"２九 全(1,9)\n"
	"１八 全(1,9)\n"
	,
	// NO.12
	"h00 y00 e00 g00 k00 u00 r00\n"
	"^k . . . . . . .^k\n"
	" .^k . . . . .^k .\n"
	" . . . .^k . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . .^k . . . .\n"
	" .^k . . . . .^k .\n"
	"^k . . . . . . .^k\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"first"
	":"
	"８一 金(9,1)\n"
	"９二 金(9,1)\n"
	"２一 金(1,1)\n"
	"１二 金(1,1)\n"
	"８一 金(8,2)\n"
	"７一 金(8,2)\n"
	"９二 金(8,2)\n"
	"７二 金(8,2)\n"
	"８三 金(8,2)\n"
	"３一 金(2,2)\n"
	"２一 金(2,2)\n"
	"３二 金(2,2)\n"
	"１二 金(2,2)\n"
	"２三 金(2,2)\n"
	"６二 金(5,3)\n"
	"５二 金(5,3)\n"
	"４二 金(5,3)\n"
	"６三 金(5,3)\n"
	"４三 金(5,3)\n"
	"５四 金(5,3)\n"
	"６六 金(5,7)\n"
	"５六 金(5,7)\n"
	"４六 金(5,7)\n"
	"６七 金(5,7)\n"
	"４七 金(5,7)\n"
	"５八 金(5,7)\n"
	"９七 金(8,8)\n"
	"８七 金(8,8)\n"
	"７七 金(8,8)\n"
	"９八 金(8,8)\n"
	"７八 金(8,8)\n"
	"８九 金(8,8)\n"
	"３七 金(2,8)\n"
	"２七 金(2,8)\n"
	"１七 金(2,8)\n"
	"３八 金(2,8)\n"
	"１八 金(2,8)\n"
	"２九 金(2,8)\n"
	"９八 金(9,9)\n"
	"８九 金(9,9)\n"
	"１八 金(1,9)\n"
	"２九 金(1,9)\n"
	,
	// NO.13
	"h00 y00 e00 g00 k00 u00 r00\n"
	"K_ . . . . . . .K_\n"
	" .K_ . . . . .K_ .\n"
	" . . . .K_ . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . .K_ . . . .\n"
	" .K_ . . . . .K_ .\n"
	"K_ . . . . . . .K_\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"second"
	":"
	"９二 金(9,1)\n"
	"８一 金(9,1)\n"
	"１二 金(1,1)\n"
	"２一 金(1,1)\n"
	"９三 金(8,2)\n"
	"８三 金(8,2)\n"
	"７三 金(8,2)\n"
	"９二 金(8,2)\n"
	"７二 金(8,2)\n"
	"８一 金(8,2)\n"
	"３三 金(2,2)\n"
	"２三 金(2,2)\n"
	"１三 金(2,2)\n"
	"３二 金(2,2)\n"
	"１二 金(2,2)\n"
	"２一 金(2,2)\n"
	"６四 金(5,3)\n"
	"５四 金(5,3)\n"
	"４四 金(5,3)\n"
	"６三 金(5,3)\n"
	"４三 金(5,3)\n"
	"５二 金(5,3)\n"
	"６八 金(5,7)\n"
	"５八 金(5,7)\n"
	"４八 金(5,7)\n"
	"６七 金(5,7)\n"
	"４七 金(5,7)\n"
	"５六 金(5,7)\n"
	"８九 金(8,8)\n"
	"７九 金(8,8)\n"
	"９八 金(8,8)\n"
	"７八 金(8,8)\n"
	"８七 金(8,8)\n"
	"３九 金(2,8)\n"
	"２九 金(2,8)\n"
	"３八 金(2,8)\n"
	"１八 金(2,8)\n"
	"２七 金(2,8)\n"
	"８九 金(9,9)\n"
	"９八 金(9,9)\n"
	"２九 金(1,9)\n"
	"１八 金(1,9)\n"
	,
	// NO.14
	"h00 y00 e00 g00 k00 u00 r00\n"
	"^u . . .^u . . .^u\n"
	" . . . . . . . . .\n"
	" . . . .h_ . . . .\n"
	" . . . . . . . . .\n"
	"^u . . . . .^h .^u\n"
	" . . . . . . . . .\n"
	" . .^u . . . . . .\n"
	" . . . . . . . . .\n"
	"^u . . .^u . . .^u\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"first"
	":"
	"８二 角(9,1)\n"
	"８二 角(9,1) 成り\n"
	"７三 角(9,1)\n"
	"７三 角(9,1) 成り\n"
	"６四 角(9,1)\n"
	"５五 角(9,1)\n"
	"４六 角(9,1)\n"
	"３七 角(9,1)\n"
	"２八 角(9,1)\n"
	"４二 角(5,1)\n"
	"４二 角(5,1) 成り\n"
	"３三 角(5,1)\n"
	"３三 角(5,1) 成り\n"
	"２四 角(5,1)\n"
	"６二 角(5,1)\n"
	"６二 角(5,1) 成り\n"
	"７三 角(5,1)\n"
	"７三 角(5,1) 成り\n"
	"８四 角(5,1)\n"
	"２二 角(1,1)\n"
	"２二 角(1,1) 成り\n"
	"３三 角(1,1)\n"
	"３三 角(1,1) 成り\n"
	"４四 角(1,1)\n"
	"５五 角(1,1)\n"
	"６六 角(1,1)\n"
	"８六 角(9,5)\n"
	"８四 角(9,5)\n"
	"７三 角(9,5)\n"
	"７三 角(9,5) 成り\n"
	"６二 角(9,5)\n"
	"６二 角(9,5) 成り\n"
	"３四 歩(3,5)\n"
	"２六 角(1,5)\n"
	"３七 角(1,5)\n"
	"４八 角(1,5)\n"
	"２四 角(1,5)\n"
	"３三 角(1,5)\n"
	"３三 角(1,5) 成り\n"
	"４二 角(1,5)\n"
	"４二 角(1,5) 成り\n"
	"６八 角(7,7)\n"
	"６六 角(7,7)\n"
	"５五 角(7,7)\n"
	"４四 角(7,7)\n"
	"３三 角(7,7)\n"
	"３三 角(7,7) 成り\n"
	"２二 角(7,7)\n"
	"２二 角(7,7) 成り\n"
	"８八 角(7,7)\n"
	"８六 角(7,7)\n"
	"８八 角(9,9)\n"
	"４八 角(5,9)\n"
	"３七 角(5,9)\n"
	"２六 角(5,9)\n"
	"６八 角(5,9)\n"
	"２八 角(1,9)\n"
	"３七 角(1,9)\n"
	"４六 角(1,9)\n"
	"５五 角(1,9)\n"
	"６四 角(1,9)\n"
	"７三 角(1,9)\n"
	"７三 角(1,9) 成り\n"
	"８二 角(1,9)\n"
	"８二 角(1,9) 成り\n"
	,
	// NO.15
	"h00 y00 e00 g00 k00 u00 r00\n"
	"u_ . . .u_ . . .u_\n"
	" . . . . . . . . .\n"
	" . . . .h_ . . . .\n"
	" . . . . . . . . .\n"
	"u_ . . . . ^h. .u_\n"
	" . . . . . . . . .\n"
	" . .u_ . . . . . .\n"
	" . . . . . . . . .\n"
	"u_ . . .u_ . . .u_\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"second"
	":"
	"８二 角(9,1)\n"
	"７三 角(9,1)\n"
	"６四 角(9,1)\n"
	"５五 角(9,1)\n"
	"４六 角(9,1)\n"
	"３七 角(9,1)\n"
	"３七 角(9,1) 成り\n"
	"２八 角(9,1)\n"
	"２八 角(9,1) 成り\n"
	"４二 角(5,1)\n"
	"３三 角(5,1)\n"
	"２四 角(5,1)\n"
	"６二 角(5,1)\n"
	"７三 角(5,1)\n"
	"８四 角(5,1)\n"
	"２二 角(1,1)\n"
	"３三 角(1,1)\n"
	"４四 角(1,1)\n"
	"５五 角(1,1)\n"
	"６六 角(1,1)\n"
	"５四 歩(5,3)\n"
	"８六 角(9,5)\n"
	"８四 角(9,5)\n"
	"７三 角(9,5)\n"
	"６二 角(9,5)\n"
	"３六 歩(3,5)\n"
	"２六 角(1,5)\n"
	"３七 角(1,5)\n"
	"３七 角(1,5) 成り\n"
	"４八 角(1,5)\n"
	"４八 角(1,5) 成り\n"
	"２四 角(1,5)\n"
	"３三 角(1,5)\n"
	"４二 角(1,5)\n"
	"６八 角(7,7)\n"
	"６八 角(7,7) 成り\n"
	"６六 角(7,7)\n"
	"５五 角(7,7)\n"
	"４四 角(7,7)\n"
	"３三 角(7,7)\n"
	"２二 角(7,7)\n"
	"８八 角(7,7)\n"
	"８八 角(7,7) 成り\n"
	"８六 角(7,7)\n"
	"８八 角(9,9)\n"
	"８八 角(9,9) 成り\n"
	"４八 角(5,9)\n"
	"４八 角(5,9) 成り\n"
	"３七 角(5,9)\n"
	"３七 角(5,9) 成り\n"
	"２六 角(5,9)\n"
	"６八 角(5,9)\n"
	"６八 角(5,9) 成り\n"
	"２八 角(1,9)\n"
	"２八 角(1,9) 成り\n"
	"３七 角(1,9)\n"
	"３七 角(1,9) 成り\n"
	"４六 角(1,9)\n"
	"５五 角(1,9)\n"
	"６四 角(1,9)\n"
	"７三 角(1,9)\n"
	"８二 角(1,9)\n"
	,
	// NO.16
	"h00 y00 e00 g00 k00 u00 r00\n"
	"^U . . .^U . . .^U\n"
	" . . . . . . . . .\n"
	" . . . .h_ . . . .\n"
	" . . . . . . . . .\n"
	"^U . . . . .^h .^U\n"
	" . . . . . . . . .\n"
	" . .^U . . . . . .\n"
	" . . . . . . . . .\n"
	"^U . . .^U . . .^U\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"first"
	":"
	"８二 馬(9,1)\n"
	"７三 馬(9,1)\n"
	"６四 馬(9,1)\n"
	"５五 馬(9,1)\n"
	"４六 馬(9,1)\n"
	"３七 馬(9,1)\n"
	"２八 馬(9,1)\n"
	"８一 馬(9,1)\n"
	"９二 馬(9,1)\n"
	"４二 馬(5,1)\n"
	"３三 馬(5,1)\n"
	"２四 馬(5,1)\n"
	"６二 馬(5,1)\n"
	"７三 馬(5,1)\n"
	"８四 馬(5,1)\n"
	"４一 馬(5,1)\n"
	"６一 馬(5,1)\n"
	"５二 馬(5,1)\n"
	"２二 馬(1,1)\n"
	"３三 馬(1,1)\n"
	"４四 馬(1,1)\n"
	"５五 馬(1,1)\n"
	"６六 馬(1,1)\n"
	"２一 馬(1,1)\n"
	"１二 馬(1,1)\n"
	"８六 馬(9,5)\n"
	"８四 馬(9,5)\n"
	"７三 馬(9,5)\n"
	"６二 馬(9,5)\n"
	"８五 馬(9,5)\n"
	"９六 馬(9,5)\n"
	"９四 馬(9,5)\n"
	"３四 歩(3,5)\n"
	"２六 馬(1,5)\n"
	"３七 馬(1,5)\n"
	"４八 馬(1,5)\n"
	"２四 馬(1,5)\n"
	"３三 馬(1,5)\n"
	"４二 馬(1,5)\n"
	"２五 馬(1,5)\n"
	"１六 馬(1,5)\n"
	"１四 馬(1,5)\n"
	"６八 馬(7,7)\n"
	"６六 馬(7,7)\n"
	"５五 馬(7,7)\n"
	"４四 馬(7,7)\n"
	"３三 馬(7,7)\n"
	"２二 馬(7,7)\n"
	"８八 馬(7,7)\n"
	"８六 馬(7,7)\n"
	"６七 馬(7,7)\n"
	"８七 馬(7,7)\n"
	"７八 馬(7,7)\n"
	"７六 馬(7,7)\n"
	"８八 馬(9,9)\n"
	"８九 馬(9,9)\n"
	"９八 馬(9,9)\n"
	"４八 馬(5,9)\n"
	"３七 馬(5,9)\n"
	"２六 馬(5,9)\n"
	"６八 馬(5,9)\n"
	"４九 馬(5,9)\n"
	"６九 馬(5,9)\n"
	"５八 馬(5,9)\n"
	"２八 馬(1,9)\n"
	"３七 馬(1,9)\n"
	"４六 馬(1,9)\n"
	"５五 馬(1,9)\n"
	"６四 馬(1,9)\n"
	"７三 馬(1,9)\n"
	"８二 馬(1,9)\n"
	"２九 馬(1,9)\n"
	"１八 馬(1,9)\n"
	,
	// NO.17
	"h00 y00 e00 g00 k00 u00 r00\n"
	"U_ . . .U_ . . .U_\n"
	" . . . . . . . . .\n"
	" . . . .h_ . . . .\n"
	" . . . . . . . . .\n"
	"U_ . . . . ^h. .U_\n"
	" . . . . . . . . .\n"
	" . .U_ . . . . . .\n"
	" . . . . . . . . .\n"
	"U_ . . .U_ . . .U_\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"second"
	":"
	"８二 馬(9,1)\n"
	"７三 馬(9,1)\n"
	"６四 馬(9,1)\n"
	"５五 馬(9,1)\n"
	"４六 馬(9,1)\n"
	"３七 馬(9,1)\n"
	"２八 馬(9,1)\n"
	"８一 馬(9,1)\n"
	"９二 馬(9,1)\n"
	"４二 馬(5,1)\n"
	"３三 馬(5,1)\n"
	"２四 馬(5,1)\n"
	"６二 馬(5,1)\n"
	"７三 馬(5,1)\n"
	"８四 馬(5,1)\n"
	"４一 馬(5,1)\n"
	"６一 馬(5,1)\n"
	"５二 馬(5,1)\n"
	"２二 馬(1,1)\n"
	"３三 馬(1,1)\n"
	"４四 馬(1,1)\n"
	"５五 馬(1,1)\n"
	"６六 馬(1,1)\n"
	"２一 馬(1,1)\n"
	"１二 馬(1,1)\n"
	"５四 歩(5,3)\n"
	"８六 馬(9,5)\n"
	"８四 馬(9,5)\n"
	"７三 馬(9,5)\n"
	"６二 馬(9,5)\n"
	"８五 馬(9,5)\n"
	"９六 馬(9,5)\n"
	"９四 馬(9,5)\n"
	"３六 歩(3,5)\n"
	"２六 馬(1,5)\n"
	"３七 馬(1,5)\n"
	"４八 馬(1,5)\n"
	"２四 馬(1,5)\n"
	"３三 馬(1,5)\n"
	"４二 馬(1,5)\n"
	"２五 馬(1,5)\n"
	"１六 馬(1,5)\n"
	"１四 馬(1,5)\n"
	"６八 馬(7,7)\n"
	"６六 馬(7,7)\n"
	"５五 馬(7,7)\n"
	"４四 馬(7,7)\n"
	"３三 馬(7,7)\n"
	"２二 馬(7,7)\n"
	"８八 馬(7,7)\n"
	"８六 馬(7,7)\n"
	"６七 馬(7,7)\n"
	"８七 馬(7,7)\n"
	"７八 馬(7,7)\n"
	"７六 馬(7,7)\n"
	"８八 馬(9,9)\n"
	"８九 馬(9,9)\n"
	"９八 馬(9,9)\n"
	"４八 馬(5,9)\n"
	"３七 馬(5,9)\n"
	"２六 馬(5,9)\n"
	"６八 馬(5,9)\n"
	"４九 馬(5,9)\n"
	"６九 馬(5,9)\n"
	"５八 馬(5,9)\n"
	"２八 馬(1,9)\n"
	"３七 馬(1,9)\n"
	"４六 馬(1,9)\n"
	"５五 馬(1,9)\n"
	"６四 馬(1,9)\n"
	"７三 馬(1,9)\n"
	"８二 馬(1,9)\n"
	"２九 馬(1,9)\n"
	"１八 馬(1,9)\n"
	,
	// NO.18
	"h00 y00 e00 g00 k00 u00 r00\n"
	"^r . . .^r . . .^r\n"
	" . . . . . . . . .\n"
	" . . . .h_ . . . .\n"
	" . . . . . . . . .\n"
	"^r. . . . .^h. .^r\n"
	" . . . . . . . . .\n"
	" . .^r . . . . . .\n"
	" . . . . . . . . .\n"
	"^r . . .^r . . .^r\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"first"
	":"
	"８一 飛(9,1)\n"
	"８一 飛(9,1) 成り\n"
	"７一 飛(9,1)\n"
	"７一 飛(9,1) 成り\n"
	"６一 飛(9,1)\n"
	"６一 飛(9,1) 成り\n"
	"９二 飛(9,1)\n"
	"９二 飛(9,1) 成り\n"
	"９三 飛(9,1)\n"
	"９三 飛(9,1) 成り\n"
	"９四 飛(9,1)\n"
	"４一 飛(5,1)\n"
	"４一 飛(5,1) 成り\n"
	"３一 飛(5,1)\n"
	"３一 飛(5,1) 成り\n"
	"２一 飛(5,1)\n"
	"２一 飛(5,1) 成り\n"
	"６一 飛(5,1)\n"
	"６一 飛(5,1) 成り\n"
	"７一 飛(5,1)\n"
	"７一 飛(5,1) 成り\n"
	"８一 飛(5,1)\n"
	"８一 飛(5,1) 成り\n"
	"５二 飛(5,1)\n"
	"５二 飛(5,1) 成り\n"
	"５三 飛(5,1)\n"
	"５三 飛(5,1) 成り\n"
	"２一 飛(1,1)\n"
	"２一 飛(1,1) 成り\n"
	"３一 飛(1,1)\n"
	"３一 飛(1,1) 成り\n"
	"４一 飛(1,1)\n"
	"４一 飛(1,1) 成り\n"
	"１二 飛(1,1)\n"
	"１二 飛(1,1) 成り\n"
	"１三 飛(1,1)\n"
	"１三 飛(1,1) 成り\n"
	"１四 飛(1,1)\n"
	"８五 飛(9,5)\n"
	"７五 飛(9,5)\n"
	"６五 飛(9,5)\n"
	"５五 飛(9,5)\n"
	"４五 飛(9,5)\n"
	"３五 飛(9,5)\n"
	"９六 飛(9,5)\n"
	"９七 飛(9,5)\n"
	"９八 飛(9,5)\n"
	"９四 飛(9,5)\n"
	"９三 飛(9,5)\n"
	"９三 飛(9,5) 成り\n"
	"９二 飛(9,5)\n"
	"９二 飛(9,5) 成り\n"
	"２五 飛(1,5)\n"
	"３五 飛(1,5)\n"
	"１六 飛(1,5)\n"
	"１七 飛(1,5)\n"
	"１八 飛(1,5)\n"
	"１四 飛(1,5)\n"
	"１三 飛(1,5)\n"
	"１三 飛(1,5) 成り\n"
	"１二 飛(1,5)\n"
	"１二 飛(1,5) 成り\n"
	"６七 飛(7,7)\n"
	"５七 飛(7,7)\n"
	"４七 飛(7,7)\n"
	"３七 飛(7,7)\n"
	"２七 飛(7,7)\n"
	"１七 飛(7,7)\n"
	"８七 飛(7,7)\n"
	"９七 飛(7,7)\n"
	"７八 飛(7,7)\n"
	"７九 飛(7,7)\n"
	"７六 飛(7,7)\n"
	"７五 飛(7,7)\n"
	"７四 飛(7,7)\n"
	"７三 飛(7,7)\n"
	"７三 飛(7,7) 成り\n"
	"７二 飛(7,7)\n"
	"７二 飛(7,7) 成り\n"
	"７一 飛(7,7)\n"
	"７一 飛(7,7) 成り\n"
	"８九 飛(9,9)\n"
	"７九 飛(9,9)\n"
	"６九 飛(9,9)\n"
	"９八 飛(9,9)\n"
	"９七 飛(9,9)\n"
	"９六 飛(9,9)\n"
	"４九 飛(5,9)\n"
	"３九 飛(5,9)\n"
	"２九 飛(5,9)\n"
	"６九 飛(5,9)\n"
	"７九 飛(5,9)\n"
	"８九 飛(5,9)\n"
	"５八 飛(5,9)\n"
	"５七 飛(5,9)\n"
	"５六 飛(5,9)\n"
	"５五 飛(5,9)\n"
	"５四 飛(5,9)\n"
	"５三 飛(5,9)\n"
	"５三 飛(5,9) 成り\n"
	"２九 飛(1,9)\n"
	"３九 飛(1,9)\n"
	"４九 飛(1,9)\n"
	"１八 飛(1,9)\n"
	"１七 飛(1,9)\n"
	"１六 飛(1,9)\n"
	,
	// NO.19
	"h00 y00 e00 g00 k00 u00 r00\n"
	"r_ . . .r_ . . .r_\n"
	" . . . . . . . . .\n"
	" . . . .h_ . . . .\n"
	" . . . . . . . . .\n"
	"r_ . . . . ^h. .r_\n"
	" . . . . . . . . .\n"
	" . .r_ . . . . . .\n"
	" . . . . . . . . .\n"
	"r_ . . .r_ . . .r_\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"second"
	":"
	"８一 飛(9,1)\n"
	"７一 飛(9,1)\n"
	"６一 飛(9,1)\n"
	"９二 飛(9,1)\n"
	"９三 飛(9,1)\n"
	"９四 飛(9,1)\n"
	"４一 飛(5,1)\n"
	"３一 飛(5,1)\n"
	"２一 飛(5,1)\n"
	"６一 飛(5,1)\n"
	"７一 飛(5,1)\n"
	"８一 飛(5,1)\n"
	"５二 飛(5,1)\n"
	"２一 飛(1,1)\n"
	"３一 飛(1,1)\n"
	"４一 飛(1,1)\n"
	"１二 飛(1,1)\n"
	"１三 飛(1,1)\n"
	"１四 飛(1,1)\n"
	"５四 歩(5,3)\n"
	"８五 飛(9,5)\n"
	"７五 飛(9,5)\n"
	"６五 飛(9,5)\n"
	"５五 飛(9,5)\n"
	"４五 飛(9,5)\n"
	"９六 飛(9,5)\n"
	"９七 飛(9,5)\n"
	"９七 飛(9,5) 成り\n"
	"９八 飛(9,5)\n"
	"９八 飛(9,5) 成り\n"
	"９四 飛(9,5)\n"
	"９三 飛(9,5)\n"
	"９二 飛(9,5)\n"
	"３六 歩(3,5)\n"
	"２五 飛(1,5)\n"
	"１六 飛(1,5)\n"
	"１七 飛(1,5)\n"
	"１七 飛(1,5) 成り\n"
	"１八 飛(1,5)\n"
	"１八 飛(1,5) 成り\n"
	"１四 飛(1,5)\n"
	"１三 飛(1,5)\n"
	"１二 飛(1,5)\n"
	"６七 飛(7,7)\n"
	"６七 飛(7,7) 成り\n"
	"５七 飛(7,7)\n"
	"５七 飛(7,7) 成り\n"
	"４七 飛(7,7)\n"
	"４七 飛(7,7) 成り\n"
	"３七 飛(7,7)\n"
	"３七 飛(7,7) 成り\n"
	"２七 飛(7,7)\n"
	"２七 飛(7,7) 成り\n"
	"１七 飛(7,7)\n"
	"１七 飛(7,7) 成り\n"
	"８七 飛(7,7)\n"
	"８七 飛(7,7) 成り\n"
	"９七 飛(7,7)\n"
	"９七 飛(7,7) 成り\n"
	"７八 飛(7,7)\n"
	"７八 飛(7,7) 成り\n"
	"７九 飛(7,7)\n"
	"７九 飛(7,7) 成り\n"
	"７六 飛(7,7)\n"
	"７五 飛(7,7)\n"
	"７四 飛(7,7)\n"
	"７三 飛(7,7)\n"
	"７二 飛(7,7)\n"
	"７一 飛(7,7)\n"
	"８九 飛(9,9)\n"
	"８九 飛(9,9) 成り\n"
	"７九 飛(9,9)\n"
	"７九 飛(9,9) 成り\n"
	"６九 飛(9,9)\n"
	"６九 飛(9,9) 成り\n"
	"９八 飛(9,9)\n"
	"９八 飛(9,9) 成り\n"
	"９七 飛(9,9)\n"
	"９七 飛(9,9) 成り\n"
	"９六 飛(9,9)\n"
	"４九 飛(5,9)\n"
	"４九 飛(5,9) 成り\n"
	"３九 飛(5,9)\n"
	"３九 飛(5,9) 成り\n"
	"２九 飛(5,9)\n"
	"２九 飛(5,9) 成り\n"
	"６九 飛(5,9)\n"
	"６九 飛(5,9) 成り\n"
	"７九 飛(5,9)\n"
	"７九 飛(5,9) 成り\n"
	"８九 飛(5,9)\n"
	"８九 飛(5,9) 成り\n"
	"５八 飛(5,9)\n"
	"５八 飛(5,9) 成り\n"
	"５七 飛(5,9)\n"
	"５七 飛(5,9) 成り\n"
	"５六 飛(5,9)\n"
	"５五 飛(5,9)\n"
	"５四 飛(5,9)\n"
	"２九 飛(1,9)\n"
	"２九 飛(1,9) 成り\n"
	"３九 飛(1,9)\n"
	"３九 飛(1,9) 成り\n"
	"４九 飛(1,9)\n"
	"４九 飛(1,9) 成り\n"
	"１八 飛(1,9)\n"
	"１八 飛(1,9) 成り\n"
	"１七 飛(1,9)\n"
	"１七 飛(1,9) 成り\n"
	"１六 飛(1,9)\n"
	,
	// NO.20
	"h00 y00 e00 g00 k00 u00 r00\n"
	"^R . . .^R . . .^R\n"
	" . . . . . . . . .\n"
	" . . . .h_ . . . .\n"
	" . . . . . . . . .\n"
	"^R . . . . .^h .^R\n"
	" . . . . . . . . .\n"
	" . .^R . . . . . .\n"
	" . . . . . . . . .\n"
	"^R . . .^R . . .^R\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"first"
	":"
	"８一 龍(9,1)\n"
	"７一 龍(9,1)\n"
	"６一 龍(9,1)\n"
	"９二 龍(9,1)\n"
	"９三 龍(9,1)\n"
	"９四 龍(9,1)\n"
	"８二 龍(9,1)\n"
	"４一 龍(5,1)\n"
	"３一 龍(5,1)\n"
	"２一 龍(5,1)\n"
	"６一 龍(5,1)\n"
	"７一 龍(5,1)\n"
	"８一 龍(5,1)\n"
	"５二 龍(5,1)\n"
	"５三 龍(5,1)\n"
	"４二 龍(5,1)\n"
	"６二 龍(5,1)\n"
	"２一 龍(1,1)\n"
	"３一 龍(1,1)\n"
	"４一 龍(1,1)\n"
	"１二 龍(1,1)\n"
	"１三 龍(1,1)\n"
	"１四 龍(1,1)\n"
	"２二 龍(1,1)\n"
	"８五 龍(9,5)\n"
	"７五 龍(9,5)\n"
	"６五 龍(9,5)\n"
	"５五 龍(9,5)\n"
	"４五 龍(9,5)\n"
	"９六 龍(9,5)\n"
	"９七 龍(9,5)\n"
	"９八 龍(9,5)\n"
	"９四 龍(9,5)\n"
	"９三 龍(9,5)\n"
	"９二 龍(9,5)\n"
	"８六 龍(9,5)\n"
	"８四 龍(9,5)\n"
	"３四 歩(3,5)\n"
	"２五 龍(1,5)\n"
	"１六 龍(1,5)\n"
	"１七 龍(1,5)\n"
	"１八 龍(1,5)\n"
	"１四 龍(1,5)\n"
	"１三 龍(1,5)\n"
	"１二 龍(1,5)\n"
	"２六 龍(1,5)\n"
	"２四 龍(1,5)\n"
	"６七 龍(7,7)\n"
	"５七 龍(7,7)\n"
	"４七 龍(7,7)\n"
	"３七 龍(7,7)\n"
	"２七 龍(7,7)\n"
	"１七 龍(7,7)\n"
	"８七 龍(7,7)\n"
	"９七 龍(7,7)\n"
	"７八 龍(7,7)\n"
	"７九 龍(7,7)\n"
	"７六 龍(7,7)\n"
	"７五 龍(7,7)\n"
	"７四 龍(7,7)\n"
	"７三 龍(7,7)\n"
	"７二 龍(7,7)\n"
	"７一 龍(7,7)\n"
	"６八 龍(7,7)\n"
	"８八 龍(7,7)\n"
	"６六 龍(7,7)\n"
	"８六 龍(7,7)\n"
	"８九 龍(9,9)\n"
	"７九 龍(9,9)\n"
	"６九 龍(9,9)\n"
	"９八 龍(9,9)\n"
	"９七 龍(9,9)\n"
	"９六 龍(9,9)\n"
	"８八 龍(9,9)\n"
	"４九 龍(5,9)\n"
	"３九 龍(5,9)\n"
	"２九 龍(5,9)\n"
	"６九 龍(5,9)\n"
	"７九 龍(5,9)\n"
	"８九 龍(5,9)\n"
	"５八 龍(5,9)\n"
	"５七 龍(5,9)\n"
	"５六 龍(5,9)\n"
	"５五 龍(5,9)\n"
	"５四 龍(5,9)\n"
	"５三 龍(5,9)\n"
	"４八 龍(5,9)\n"
	"６八 龍(5,9)\n"
	"２九 龍(1,9)\n"
	"３九 龍(1,9)\n"
	"４九 龍(1,9)\n"
	"１八 龍(1,9)\n"
	"１七 龍(1,9)\n"
	"１六 龍(1,9)\n"
	"２八 龍(1,9)\n"
	,
	// NO.21
	"h00 y00 e00 g00 k00 u00 r00\n"
	"R_ . . .R_ . . .R_\n"
	" . . . . . . . . .\n"
	" . . . .h_ . . . .\n"
	" . . . . . . . . .\n"
	"R_ . . . . ^h. .R_\n"
	" . . . . . . . . .\n"
	" . .R_ . . . . . .\n"
	" . . . . . . . . .\n"
	"R_ . . .R_ . . .R_\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"second"
	":"
	"８一 龍(9,1)\n"
	"７一 龍(9,1)\n"
	"６一 龍(9,1)\n"
	"９二 龍(9,1)\n"
	"９三 龍(9,1)\n"
	"９四 龍(9,1)\n"
	"８二 龍(9,1)\n"
	"４一 龍(5,1)\n"
	"３一 龍(5,1)\n"
	"２一 龍(5,1)\n"
	"６一 龍(5,1)\n"
	"７一 龍(5,1)\n"
	"８一 龍(5,1)\n"
	"５二 龍(5,1)\n"
	"４二 龍(5,1)\n"
	"６二 龍(5,1)\n"
	"２一 龍(1,1)\n"
	"３一 龍(1,1)\n"
	"４一 龍(1,1)\n"
	"１二 龍(1,1)\n"
	"１三 龍(1,1)\n"
	"１四 龍(1,1)\n"
	"２二 龍(1,1)\n"
	"５四 歩(5,3)\n"
	"８五 龍(9,5)\n"
	"７五 龍(9,5)\n"
	"６五 龍(9,5)\n"
	"５五 龍(9,5)\n"
	"４五 龍(9,5)\n"
	"９六 龍(9,5)\n"
	"９七 龍(9,5)\n"
	"９八 龍(9,5)\n"
	"９四 龍(9,5)\n"
	"９三 龍(9,5)\n"
	"９二 龍(9,5)\n"
	"８六 龍(9,5)\n"
	"８四 龍(9,5)\n"
	"３六 歩(3,5)\n"
	"２五 龍(1,5)\n"
	"１六 龍(1,5)\n"
	"１七 龍(1,5)\n"
	"１八 龍(1,5)\n"
	"１四 龍(1,5)\n"
	"１三 龍(1,5)\n"
	"１二 龍(1,5)\n"
	"２六 龍(1,5)\n"
	"２四 龍(1,5)\n"
	"６七 龍(7,7)\n"
	"５七 龍(7,7)\n"
	"４七 龍(7,7)\n"
	"３七 龍(7,7)\n"
	"２七 龍(7,7)\n"
	"１七 龍(7,7)\n"
	"８七 龍(7,7)\n"
	"９七 龍(7,7)\n"
	"７八 龍(7,7)\n"
	"７九 龍(7,7)\n"
	"７六 龍(7,7)\n"
	"７五 龍(7,7)\n"
	"７四 龍(7,7)\n"
	"７三 龍(7,7)\n"
	"７二 龍(7,7)\n"
	"７一 龍(7,7)\n"
	"６八 龍(7,7)\n"
	"８八 龍(7,7)\n"
	"６六 龍(7,7)\n"
	"８六 龍(7,7)\n"
	"８九 龍(9,9)\n"
	"７九 龍(9,9)\n"
	"６九 龍(9,9)\n"
	"９八 龍(9,9)\n"
	"９七 龍(9,9)\n"
	"９六 龍(9,9)\n"
	"８八 龍(9,9)\n"
	"４九 龍(5,9)\n"
	"３九 龍(5,9)\n"
	"２九 龍(5,9)\n"
	"６九 龍(5,9)\n"
	"７九 龍(5,9)\n"
	"８九 龍(5,9)\n"
	"５八 龍(5,9)\n"
	"５七 龍(5,9)\n"
	"５六 龍(5,9)\n"
	"５五 龍(5,9)\n"
	"５四 龍(5,9)\n"
	"４八 龍(5,9)\n"
	"６八 龍(5,9)\n"
	"２九 龍(1,9)\n"
	"３九 龍(1,9)\n"
	"４九 龍(1,9)\n"
	"１八 龍(1,9)\n"
	"１七 龍(1,9)\n"
	"１六 龍(1,9)\n"
	"２八 龍(1,9)\n"
	,
	// NO.22
	"h00 y00 e00 g00 k00 u00 r00\n"
	"^o . . . . . . .^o\n"
	" .^o . . . . .^o .\n"
	" . . . .^o . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . .^o . . . .\n"
	" .^o . . . . .^o .\n"
	"^o . . . . . . .^o\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"first"
	":"
	"８一 玉(9,1)\n"
	"９二 玉(9,1)\n"
	"２一 玉(1,1)\n"
	"１二 玉(1,1)\n"
	"８一 玉(8,2)\n"
	"７一 玉(8,2)\n"
	"９二 玉(8,2)\n"
	"７二 玉(8,2)\n"
	"９三 玉(8,2)\n"
	"８三 玉(8,2)\n"
	"７三 玉(8,2)\n"
	"３一 玉(2,2)\n"
	"２一 玉(2,2)\n"
	"３二 玉(2,2)\n"
	"１二 玉(2,2)\n"
	"３三 玉(2,2)\n"
	"２三 玉(2,2)\n"
	"１三 玉(2,2)\n"
	"６二 玉(5,3)\n"
	"５二 玉(5,3)\n"
	"４二 玉(5,3)\n"
	"６三 玉(5,3)\n"
	"４三 玉(5,3)\n"
	"６四 玉(5,3)\n"
	"５四 玉(5,3)\n"
	"４四 玉(5,3)\n"
	"６六 玉(5,7)\n"
	"５六 玉(5,7)\n"
	"４六 玉(5,7)\n"
	"６七 玉(5,7)\n"
	"４七 玉(5,7)\n"
	"６八 玉(5,7)\n"
	"５八 玉(5,7)\n"
	"４八 玉(5,7)\n"
	"９七 玉(8,8)\n"
	"８七 玉(8,8)\n"
	"７七 玉(8,8)\n"
	"９八 玉(8,8)\n"
	"７八 玉(8,8)\n"
	"８九 玉(8,8)\n"
	"７九 玉(8,8)\n"
	"３七 玉(2,8)\n"
	"２七 玉(2,8)\n"
	"１七 玉(2,8)\n"
	"３八 玉(2,8)\n"
	"１八 玉(2,8)\n"
	"３九 玉(2,8)\n"
	"２九 玉(2,8)\n"
	"９八 玉(9,9)\n"
	"８九 玉(9,9)\n"
	"１八 玉(1,9)\n"
	"２九 玉(1,9)\n"
	,
	// NO.23
	"h00 y00 e00 g00 k00 u00 r00\n"
	"o_ . . . . . . .o_\n"
	" .o_ . . . . .o_ .\n"
	" . . . .o_ . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . .o_ . . . .\n"
	" .o_ . . . . .o_ .\n"
	"o_ . . . . . . .o_\n"
	"h00 y00 e00 g00 k00 u00 r00\n"
	"second"
	":"
	"８一 玉(9,1)\n"
	"９二 玉(9,1)\n"
	"２一 玉(1,1)\n"
	"１二 玉(1,1)\n"
	"８一 玉(8,2)\n"
	"７一 玉(8,2)\n"
	"９二 玉(8,2)\n"
	"７二 玉(8,2)\n"
	"９三 玉(8,2)\n"
	"８三 玉(8,2)\n"
	"７三 玉(8,2)\n"
	"３一 玉(2,2)\n"
	"２一 玉(2,2)\n"
	"３二 玉(2,2)\n"
	"１二 玉(2,2)\n"
	"３三 玉(2,2)\n"
	"２三 玉(2,2)\n"
	"１三 玉(2,2)\n"
	"６二 玉(5,3)\n"
	"５二 玉(5,3)\n"
	"４二 玉(5,3)\n"
	"６三 玉(5,3)\n"
	"４三 玉(5,3)\n"
	"６四 玉(5,3)\n"
	"５四 玉(5,3)\n"
	"４四 玉(5,3)\n"
	"６六 玉(5,7)\n"
	"５六 玉(5,7)\n"
	"４六 玉(5,7)\n"
	"６七 玉(5,7)\n"
	"４七 玉(5,7)\n"
	"６八 玉(5,7)\n"
	"５八 玉(5,7)\n"
	"４八 玉(5,7)\n"
	"９七 玉(8,8)\n"
	"８七 玉(8,8)\n"
	"７七 玉(8,8)\n"
	"９八 玉(8,8)\n"
	"７八 玉(8,8)\n"
	"８九 玉(8,8)\n"
	"７九 玉(8,8)\n"
	"３七 玉(2,8)\n"
	"２七 玉(2,8)\n"
	"１七 玉(2,8)\n"
	"３八 玉(2,8)\n"
	"１八 玉(2,8)\n"
	"３九 玉(2,8)\n"
	"２九 玉(2,8)\n"
	"９八 玉(9,9)\n"
	"８九 玉(9,9)\n"
	"１八 玉(1,9)\n"
	"２九 玉(1,9)\n"
};