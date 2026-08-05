def minichicken_to_chicken(code: str) -> str:
    res = []
    for n in code.split():
        res.append(" ".join("chicken" for _ in range(int(n))))
    return "\n".join(res)
import sys
_,infn,outfn=sys.argv
code=open(infn).read()
code=minichicken_to_chicken(code)
open(outfn,'w').write(code)

