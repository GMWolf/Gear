<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.3.3" name="Sprites" tilewidth="89" tileheight="89" tilecount="13" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0">
  <image width="32" height="40" source="ship1.png"/>
  <objectgroup draworder="index" id="2">
   <object id="5" name="collision" x="0" y="8" width="32" height="32">
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
 <tile id="1">
  <image width="16" height="51" source="ship2.png"/>
  <objectgroup draworder="index" id="2">
   <object id="1" name="collision" x="0" y="0" width="16" height="51"/>
   <object id="2" name="gun" x="8" y="5">
    <point/>
   </object>
  </objectgroup>
 </tile>
 <tile id="2">
  <image width="89" height="89" source="explosion/explosion_0.png"/>
  <objectgroup draworder="index" id="2">
   <object id="1" name="origin" x="44.5" y="44.5">
    <point/>
   </object>
  </objectgroup>
  <animation>
   <frame tileid="2" duration="16"/>
   <frame tileid="3" duration="16"/>
   <frame tileid="4" duration="16"/>
   <frame tileid="5" duration="16"/>
   <frame tileid="6" duration="16"/>
   <frame tileid="7" duration="16"/>
   <frame tileid="8" duration="16"/>
   <frame tileid="9" duration="16"/>
   <frame tileid="10" duration="16"/>
  </animation>
 </tile>
 <tile id="3">
  <image width="89" height="89" source="explosion/explosion_1.png"/>
 </tile>
 <tile id="4">
  <image width="89" height="89" source="explosion/explosion_2.png"/>
 </tile>
 <tile id="5">
  <image width="89" height="89" source="explosion/explosion_3.png"/>
 </tile>
 <tile id="6">
  <image width="89" height="89" source="explosion/explosion_4.png"/>
 </tile>
 <tile id="7">
  <image width="89" height="89" source="explosion/explosion_5.png"/>
 </tile>
 <tile id="8">
  <image width="89" height="89" source="explosion/explosion_6.png"/>
 </tile>
 <tile id="9">
  <image width="89" height="89" source="explosion/explosion_7.png"/>
 </tile>
 <tile id="10">
  <image width="89" height="89" source="explosion/explosion_8.png"/>
 </tile>
 <tile id="11">
  <image width="89" height="89" source="explosion/explosion_9.png"/>
 </tile>
 <tile id="12">
  <image width="7" height="13" source="bullet_blue1.png"/>
  <objectgroup draworder="index" id="2">
   <object id="1" name="collision" x="0" y="0" width="7" height="7">
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
</tileset>
