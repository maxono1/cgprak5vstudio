#include "Terrain.h"
#include "rgbimage.h"
#include "Terrainshader.h"

Terrain::Terrain(const char* HeightMap, const char* DetailMap1, const char* DetailMap2) : Size(10,1,10)
{
    if(HeightMap && DetailMap1 && DetailMap2)
    {
        bool loaded = load( HeightMap, DetailMap1, DetailMap2);
        if(!loaded)
            throw std::exception();
    }
}

Terrain::~Terrain()
{
    
}

bool Terrain::load( const char* HeightMap, const char* DetailMap1, const char* DetailMap2)
{
    if( !HeightTex.load(HeightMap) )
        return false;
    if( !DetailTex[0].load(DetailMap1) )
        return false;
    if( !DetailTex[1].load(DetailMap2) )
        return false;
    
    const RGBImage* heightmap = HeightTex.getRGBImage();
    //constexpr unsigned int widthOfHeightmap = heightmap->width();
    //constexpr unsigned int heightOfHeightmap = heightmap->height();
    //Vector vectors[][] = new Vector[heightmap->width()][heightmap->height()];

    //array zuweisen
    Vector** vectors = new Vector *[heightmap->width()];
    for (size_t i = 0; i < heightmap->width(); i++)
    {   
        vectors[i] = new Vector[heightmap->height()];
    }
    
    float width = heightmap->width();
    float height = heightmap->height();

    // Werden benoetigt sonst klappt das anzeigen nicht
    float xnorm = width / 2;
    float znorm = height / 2;

    //array befüllen mit den pixelwerten und ihrer position
    for (size_t i = 0; i < heightmap->width(); i++)
    {
        for (size_t j = 0; j < heightmap->height(); j++)
        {
            // wieso durch height wtfffffff
            //vectors[i][j] = Vector(i - xnorm, heightmap->getPixelColor(i, j).R, (j - znorm));
            vectors[i][j] = Vector((i - xnorm) / width, heightmap->getPixelColor(i, j).R, (j - znorm) / height);
            //std::cout << vectors[i][j].Y << '\n';
        }
    }
    //normalen array
    Vector** normies = new Vector * [heightmap->width()];
    for (size_t i = 0; i < heightmap->width(); i++)
    {
        normies[i] = new Vector[heightmap->height()];
    }

    for (size_t i = 0; i < heightmap->width(); i++)
    {
        for (size_t j = 0; j < heightmap->height(); j++)
        {
            normies[i][j] = Vector(0, 0, 0);
        }
    }

    //für jeden vertex diese normale von jedem dreieck hinzuaddieren, 
    //ganz am ende durchgehen und nochmal normalisieren
    
    /*
            //VB.addNormal(); woher die normale berechnen?
            VB.addTexcoord0(0, 0); // das später für textur ändern
            VB.addVertex(i,heightmap->getPixelColor(i, j).R,j); //da es greyscale images sind(r=b=g), wird R verwendet
            //std::cout << "R:" << heightmap->getPixelColor(i, j).R << ", G:" << heightmap->getPixelColor(i, j).G << ", B:" << heightmap->getPixelColor(i, j).G <<'\n';
            */
    //der vertexbuffer kann erst hier gefüllt werden, da wir sicherstellen müssen, 
    //dass die benachbarten vertexes schon bekannte werte haben, um die normalen zu berechnen
    
    //tips: erstmal indexbuffer implementieren, 17 millionen vertexes sind zu groß und führen
    //zum absturz

    //i , i+1, i + zeile in den indexbuffer

    //die normalen dann aufsummieren 



    
    for (size_t x = 0; x < heightmap->width(); x++)
    {
        for (size_t y = 0; y < heightmap->height(); y++)
        {
            if (x != heightmap->width() - 1 && y != heightmap->height() - 1) {
                
                //Punkt A ist der in der Schleife
                Vector punktA = vectors[x][y];
                Vector punktB = vectors[x + 1][y + 1];
                Vector punktC = vectors[x][y + 1];
                Vector punktD = vectors[x + 1][y];

                //berechnen des richtungsvektors von A nach 
                Vector AB = punktB - punktA;
                Vector BC = punktC - punktB;

                Vector normalBottom = BC.cross(AB);

                //add the normal of A
                normies[x][y] += normalBottom;
                //of B
                normies[x + 1][y + 1] += normalBottom;
                //of C
                normies[x][y + 1] += normalBottom;

                

                Vector AD = punktD - punktA;
                Vector DB = punktB - punktD;

                Vector normalTop = DB.cross(AD);

                //add to normal of A
                normies[x][y] += normalTop;
                //of B
                normies[x + 1][y + 1] += normalTop;
                //of D
                normies[x + 1][y] += normalTop;


            }

        }
    }

    for (size_t i = 0; i < heightmap->width(); i++)
    {
        for (size_t j = 0; j < heightmap->height(); j++)
        {
            normies[i][j].normalize();
        }
    }
    VB.begin();

    for (size_t x = 0; x < heightmap->width(); x++)
    {
        for (size_t y = 0; y < heightmap->height(); y++)
        {
            VB.addNormal(normies[x][y]);
            //VB.addNormal(0,1,0);
            VB.addTexcoord0(0,0);
            VB.addVertex(vectors[x][y]);
        }
    }
    VB.end();
    
    IB.begin();
    for (size_t x = 0; x < (heightmap->width() ); x++)
    {
        for (size_t y = 0; y < heightmap->height(); y++)
        {
            if (x != heightmap->width() - 1 && y != heightmap->height() - 1) {
                IB.addIndex(x+y*heightmap->width());
                IB.addIndex(x+1 + y * heightmap->width());
                IB.addIndex(x + 1 + (y+1) * heightmap->width());

                IB.addIndex(x + y * heightmap->width());
                IB.addIndex(x + 1 + (y + 1) * heightmap->width());
                IB.addIndex(x + (y+1)* heightmap->width());
            }
        }
        
    }
    IB.end();

    // TODO: add code for creating terrain model
    for (size_t i = 0; i < heightmap->width(); i++)
    {
        delete[] vectors[i];
    }
    delete[] vectors;
    
    return true;
}

void Terrain::shader( BaseShader* shader, bool deleteOnDestruction )
{
    BaseModel::shader(shader, deleteOnDestruction);
}

void Terrain::draw(const BaseCamera& Cam)
{
    applyShaderParameter();
    BaseModel::draw(Cam);

    VB.activate();
    IB.activate();

    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);

    IB.deactivate();
    VB.deactivate();
    // TODO: add code for drawing index and vertex buffers
}

void Terrain::applyShaderParameter()
{
    TerrainShader* Shader = dynamic_cast<TerrainShader*>(BaseModel::shader());
    if(!Shader)
        return;
    
    Shader->mixTex(&MixTex);
    for(int i=0; i<2; i++)
        Shader->detailTex(i,&DetailTex[i]);
    Shader->scaling(Size);
    
    // TODO: add additional parameters if needed..
}

// min Val = 0.f ; Max = 1.f
float Terrain::normalizeVals(float& in) {
    float erg = (in - 0.f) / (1.f - 0.f);


    return erg;
}

/* Hasefriedhof
//dreiecke 1&2
            if (x != 0 && y != 0) {
                
                Vector normal1;
                Vector normal2;
                Vector a1 = Vector(;
                Vector a2;
                Vector b1;
                Vector b2;

            }
            //dreieck 3
            if (x != heightmap->width() - 1 && y != 0) {

            }
            //dreieck 6
            if (x != 0 && y != heightmap->height()) {

            }

*/

/*
                VB.addNormal(normalBottom);
                VB.addTexcoord0(0, 0);
                VB.addVertex(punktA);

                VB.addNormal(normalBottom);
                VB.addTexcoord0(0, 0);
                VB.addVertex(punktB);

                VB.addNormal(normalBottom);
                VB.addTexcoord0(0, 0);
                VB.addVertex(punktC);*/
                /*
                VB.addNormal(normalTop);
                VB.addTexcoord0(0, 0);
                VB.addVertex(punktA);

                VB.addNormal(normalTop);
                VB.addTexcoord0(0, 0);
                VB.addVertex(punktD);

                VB.addNormal(normalTop);
                VB.addTexcoord0(0, 0);
                VB.addVertex(punktB);*/