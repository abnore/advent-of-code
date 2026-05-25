#include "main.h"
#include <string.h>

/* forward searching will explode in complexity. We need to shrink the string,
 * working backwards. First attempt bore no fruit. Reversing got stuck, i cant
 * brute-force it...
 *
 * Looked longer at the string, and good thing i started writing my json lexer,
 * because i noticed i can tokenize the input and that Rn and Ar always came in
 * pairs, and Y serves as a seperator. Therefore i can replace them as such
 *   Rn = (
 *   Ar = )
 *   Y  = ,
 * This groups the right side to always groups of two, but sometimes its group
 * is a cluster (F,F). This cluster can be counted, but then subtract one for
 * each token, and count Y twice so that its always counts as 1 per cluster, 2
 * on the right side always. therefore each step collapses 2 groupd into one,
 * making step counting easy!

 * Therefore the minimum number of steps can be calculated by counting the total
 * tokens minus special tokens :
 *
 *   total tokens - '(' - ')' - 2*',' - 1
 */

// static const char *input = "ORnPBPMgArCaCaCaSiThCaCaSiThCaCaPBSiRnFArRnFArCaCaSiThCaCaSiThCaCaCaCaCaCaSiRnFYFArSiRnMgArCaSiRnPTiTiBFYPBFArSiRnCaSiRnTiRnFArSiAlArPTiBPTiRnCaSiAlArCaPTiTiBPMgYFArPTiRnFArSiRnCaCaFArRnCaFArCaSiRnSiRnMgArFYCaSiRnMgArCaCaSiThPRnFArPBCaSiRnMgArCaCaSiThCaSiRnTiMgArFArSiThSiThCaCaSiRnMgArCaCaSiRnFArTiBPTiRnCaSiAlArCaPTiRnFArPBPBCaCaSiThCaPBSiThPRnFArSiThCaSiThCaSiThCaPTiBSiRnFYFArCaCaPRnFArPBCaCaPBSiRnTiRnFArCaPRnFArSiRnCaCaCaSiThCaRnCaFArYCaSiRnFArBCaCaCaSiThFArPBFArCaSiRnFArRnCaCaCaFArSiRnFArTiRnPMgArF";

static const char *input = "O(PBPMg)CaCaCaSiThCaCaSiThCaCaPBSi(F)(F)CaCaSiThCaCaSiThCaCaCaCaCaCaSi(F,F)Si(Mg)CaSi(PTiTiBF,PBF)Si(CaSi(Ti(F)SiAl)PTiBPTi(CaSiAl)CaPTiTiBPMg,F)PTi(F)Si(CaCaF)(CaF)CaSi(Si(Mg)F,CaSi(Mg)CaCaSiThP(F)PBCaSi(Mg)CaCaSiThCaSi(TiMg)F)SiThSiThCaCaSi(Mg)CaCaSi(F)TiBPTi(CaSiAl)CaPTi(F)PBPBCaCaSiThCaPBSiThP(F)SiThCaSiThCaSiThCaPTiBSi(F,F)CaCaP(F)PBCaCaPBSi(Ti(F)CaP(F)Si(CaCaCaSiThCa(CaF),CaSi(F)BCaCaCaSiThF)PBF)CaSi(F)(CaCaCaF)Si(F)Ti(PMg)F";
int pt2(FILE *fp)
{
    (void)fp;

    int tokens = 0;
    int rn = 0;
    int ar = 0;
    int y = 0;

    for (int i = 0; input[i] != '\0'; i++)
    {
        char tok[3] = {0};
        tok[0] = input[i];
        // Every token start with uppercase, with an addition lowercase
        // sometimes, skip those
        if ( input[i+1] >= 'a' && input[i+1] <= 'z') {
            tok[1] = input[++i];
        }
        tokens++;
        if (!strcmp(tok, "("))      rn++;
        else if (!strcmp(tok, ")")) ar++;
        else if (!strcmp(tok, ","))  y++;
    }

    return (tokens - rn - ar - (2 * y) - 1);
}
