## 1. Přednáška

Parametrické vyjádření přímky:
X(t) = A + _u_\*t

Skalární součin (dot product):
_a_ \* _b_ = |_a_||_b_| cos alpha

Pokud jsou jednotkové, tak je to cos úhlu mezi vektory

Nebo normálně:
_a_ \* _b_ = *a*1*b*1 + *a*2*b*2 + *a*3*b*3

## 2. Přednáška

VBO - Vertex Buffer Object - data jsou zde uložena

VAO - Vertex Array Object - definuje, jak objekt dělit

## 3. Přednáška

Odvození rotace:
![Rotation by rotating axes](https://upload.patrick115.eu/screenshot/e9f8122f63.png)

Translace:

$$
\begin{bmatrix}
x' \\
y'
\end{bmatrix}
=
\begin{bmatrix}
x \\
y
\end{bmatrix}
+
\begin{bmatrix}
dx \\
dy
\end{bmatrix}
$$

Rotace:

$$
\begin{bmatrix}
x' \\
y''
\end{bmatrix}
=
\begin{bmatrix}
cos \alpha & -sin \alpha \\
sin \alpha & cos \alpha
\end{bmatrix}
*
\begin{bmatrix}
x \\
y
\end{bmatrix}
$$

\newpage
Změna měřítka:

$$
\begin{bmatrix}
x \\
y
\end{bmatrix}
=
\begin{bmatrix}
sx & 0 \\
0 & sy
\end{bmatrix}
*
\begin{bmatrix}
x \\
y
\end{bmatrix}
$$

-> translace má +, tedy když je budeme skládat, musíme uzávorkovat

brání mám tedy předem vypočtení modelové matice a museli bychom všechno posílat na GPU
Případy:

- Mám jeden bod a jednu transformaci, poslat na GPU a vypočítat tam
- Mám tisíc bodů a jednu transformaci, poslat na GPU a počítat tam
- Mám jeden bod a tisíc transformací, poslat na GPU a počítat tam
- Mám tisíc transformací a tisíc bodů, předpočítat dopředu na CPU a na GPU převést body do homogenního souřadného (projektivní prostor) systému a počítat tam

Rozšířím všechno o jednu dimenzi, tedy body z vec3 na vec4 pomocí w - 4. souřadnice, tím přenásobím všechny body, poté aplikuju transformace v mat4 a poté převod zpátky do vec3, tak že xyz podělím w. Typicky volíme w 1, ale pozor může po transformacích vyjít jinak, tedy nezahazovat, ale podělit!!!

Sinus začíná na 0, tedy sin0pi = 0. sin1/2pi = 1. sin1pi = 0
Cosinus začíná na 1, tey cos0pi = 1. cos1/2pi = 0. cos1pi = 1

Souřadné systémy:
Object Space -> Modelová matice (Model) -> World Space -> Pohledová matice (View) -> Camera space -> Projekční (Projection) -> Space -1 - 1 -> Rozměry okna -> Window space

## 4. Přednáška

Nárys, Bokorys, Půdorys:
Nárys - Ze předu, tedy zahodím X -> Z,Y
Půdorys - pohled s hora, tedy zahodim Y -> X,Z
Bokorys - Pohled z boku, tedy zahodím Z -> X,Y

### Rotace:

![Rotations](/home/patrick115/Obrázky/ZPG_rot.png)

\newpage

## 5. Přednáška

### Observer:

![Observer](https://upload.patrick115.eu/screenshot/cfdac0e2c6.png)

\newpage

### Shadery:

#### Lambert:

Intenzita je větší, čím je menší úhel mezi normálou a směrem ke světlu, tím že počítáme dot product a vektory jsou jednotkové, tak počítáme cos úhlu mezi vektory a cos v 0 je 1, tedy největší. zase když je úhel menší, tak tak cosinus klesá s rostoucím úhlem a max limituje minusovou cast cosinusu nad 90°

$$
I = max(dot(lightVector, worldNormal), 0.0);
$$

#### Phong:

Skládá se z:

- ambientní složky
- diffuzní složky (v podstatě lambert)
- speculární (zrcadlové)

Zrcadlová složka:

$$
Is = max(dot(reflect(lightVec, normalVec), cameraVec), 0.0);
$$

Speculární složka se ještě mocní například na 32

I - intenzita světla (světelná složka)
r - koeficient odrazu

Výpočty:

- Ambientní:
  $$ I_{ambient} = I_a * r_a $$
- Diflůzní:
  $$ I_{diffuse} = I_d * r_d * max(0, n * l) $$
- Zrcadlová:
  r -> reflect vector
  c -> camera vector
  $$ I_{specular} = I_s * r_s * max(0, r * c)^s $$

$$ I_v = I_ar_a + \sum_{i=0}^{n} I_{d,i}r_dcos\alpha_i + I_{s,i}r_scos^h\phi_i  $$

- s = shininess constant

#### Blinn-Phong:
Místo zrcadlové je zde halfway vector:
$$ normalize(lightDir + viewDir); $$
$$ I_{specular} = I_s * r_smax(0, (n*h))^s $$

## 6. Přednáška 

![Composite](https://upload.patrick115.eu/screenshot/19d0132ece.png)

![Normal matrix calc](https://upload.patrick115.eu/screenshot/657a1fd609.png)

## 7. Přednáška

Point light -> bodové světlo, dáno jen pozicí

Směrové -> Je dáno jen vektorem (směrem)

Reflektor -> Je dáno pozicí, směrem a úhlem
$$ if (dot(v_{light}, v_{frag}) > cos(radians(angle)) //ted pridat svetlo, jinak ne $$

Když chceme udělat útlum
$$ float intens = (dotLF - alpha) / (1 - alpha); $$

## 9. Přednáška

![Factory](https://upload.patrick115.eu/screenshot/909f2432fc.png)

malířův alg:
- Podle Z souřadnice se ta vzádlenější vyrenderuje první
- Pokud se nepřekrývají, tak na tom nezáleží
- Závislé na sort algoritmech
- Nevýhody: Když se plochy překrývají na vzájem

Z-buffer:
- Array o velikosti pixelu
- Jsou zde cisla 0 - 1
- 1 = nejdal vzdaleny a 0 primo v nas
- Funguje tak ze porovna jestli je hodnota v z bufferu mensi, jestli ano, prekresli data, jinak ne
