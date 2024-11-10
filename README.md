# C++-Rest-API-Framework
<p align="right">
  <h4 align="right">Tătar Ioan  Dan</h4>
  <h4 align="right">Gîngu Octavian Grigorie</h4>
</p>


# 1. Introducere

## 1.1 Scopul documentului
- scopul acestui document este de a defini specificațiile tehnice și detaliile de concepție pentru implementarea unui framework REST API în C++
- va detalia arhitectura, cerințele, și descrierea componentelor esențiale ale sistemului

## 1.2 Obiectivul proiectului
- obiectivul proiectului este de a dezvolta un framework REST API care să permită crearea de endpoint-uri și procesarea cererilor HTTP
- framework-ul va fi proiectat pentru a permite funcționalități de bază, cum ar fi rutarea cererilor, manipularea răspunsurilor și suport pentru metode HTTP standard (GET, POST, PUT, DELETE)

## 1.3 Domeniul de aplicare
- domeniile de aplicare pentru acest framework sunt aplicatiile simple, prototipuri si microservicii interne



# 2. Descrierea generala a sistemului

## 2.1 Prezentare generala
  Un API este un set de definiții și protocoale pentru construirea și integrarea aplicațiilor software. Uneori este denumit un contract între un furnizor de informații și un utilizator de informații, stabilind conținutul necesar de la consumator (apelul) și conținutul necesar de la producător (răspunsul). De exemplu, designul unui API pentru un serviciu de prognoză meteo ar putea specifica faptul că utilizatorul trebuie să furnizeze un cod poștal, iar producătorul va răspunde cu un răspuns în două părți, prima fiind temperatura maximă și a doua temperatura minimă.
	Cu alte cuvinte, dacă dorești să interacționezi cu un computer sau un sistem pentru a obține informații sau pentru a realiza o funcție, un API te ajută să comunici ce vrei astfel încât sistemul să poată înțelege și îndeplini cererea.
	Poți să te gândești la un API ca la un mediator între utilizatori sau clienți și resursele sau serviciile web pe care doresc să le acceseze. De asemenea, este un mod prin care o organizație poate partaja resurse și informații, menținând în același timp securitatea, controlul și autentificarea—determinând cine primește acces la ce anume.
	Un framework pentru a ajuta la crearea de REST APIs este un set de metode și membri care sprijină developer-ul în dezvoltarea aplicațiilor web de acest fel.
	Framework-ul REST API propus va fi construit pe baza limbajului C++ și va oferi funcționalități esențiale pentru gestionarea cererilor și răspunsurilor HTTP. Acesta va implementa metodele de bază ale protocolului HTTP, precum GET, POST, PUT și DELETE, permițând dezvoltatorilor să definească endpoint-uri personalizate și să gestioneze logica aferentă fiecărei cereri. Arhitectura sistemului va fi modulară, incluzând componente separate pentru serverul HTTP, parserul cererilor, routerul, generarea răspunsurilor și serializatorul/deserializatorul JSON.
	Prin implementarea acestui framework, se urmărește nu doar facilitarea dezvoltării aplicațiilor, ci și învățarea profundă a conceptelor fundamentale ale protocoalelor de rețea, gestionarea cererilor HTTP și manipularea datelor, oferind un cadru solid pentru proiecte viitoare în domeniul dezvoltării software.

 ## 2.2 Diagrama arhitecturii
 Client --> Server HTTP --> Parser HTTP --> Middleware-uri --> Router --> Generator Răspuns --> Server HTTP --> Client



 # 3. Cerinte specifice

 ## 3.1 Cerinte de baza pentru server HTTP
  Serverul HTTP trebuie să poată primi și interpreta cereri HTTP, să suporte metodele de bază precum GET, POST, PUT și DELETE și să trimită răspunsuri corecte către client. De asemenea, trebuie să fie capabil să gestioneze conexiuni multiple, fie sincron, fie asincron, pentru a răspunde eficient la cereri. În plus, serverul trebuie să asigure gestionarea corectă a erorilor și să permită extensibilitatea pentru integrarea middleware-urilor și a altor funcționalități suplimentare.

## 3.2 Rute si managementul cererilor
  Rutele definesc punctele de acces (URL-uri) și mapează cererile HTTP la funcțiile specifice (handler-e) care le gestionează, în funcție de metoda cererii (GET, POST, etc.). Managementul cererilor implică direcționarea cererilor către handler-ele corespunzătoare și prelucrarea datelor pentru a genera răspunsul potrivit. Sistemul de rutare trebuie să fie flexibil, permițând adăugarea de noi rute și aplicarea de middleware-uri pentru pre-procesarea cererilor.

## 3.3 Serializare si deserializare JSON
  Serializarea JSON transformă datele din structuri interne (precum obiecte sau structuri C++) într-un format JSON, ușor de transmis către client. Deserializarea JSON face conversia inversă, transformând datele JSON primite într-o formă utilizabilă intern de către server. Aceste procese sunt esențiale pentru comunicarea eficientă într-un API REST, permițând schimbul de date structurate între client și server.

## 3.4 Gestionarea erorilor
  Gestionarea erorilor presupune identificarea, capturarea și tratarea erorilor apărute în timpul procesării cererilor HTTP, oferind răspunsuri descriptive și coduri de stare adecvate (ex. 404 pentru „Not Found” sau 500 pentru „Internal Server Error”). Un sistem robust de gestionare a erorilor ajută la menținerea stabilității serverului și îmbunătățește experiența utilizatorului prin mesaje de eroare clare. În plus, gestionarea erorilor poate include logarea automată a incidentelor pentru diagnosticare și depanare.



# 4. Arhitectura sistemului

## 4.1 Server HTTP
  Serverul HTTP gestionează conexiunile și primește cererile HTTP de la clienți, inițiind procesul de tratare a acestora. El stabilește legătura cu fiecare client, gestionează conexiunile multiple și distribuie cererile către componentele interne corespunzătoare. Serverul asigură, de asemenea, trimiterea răspunsurilor înapoi la client.

## 4.2 Parser cereri HTTP
  Parserul de cereri HTTP interpretează cererile primite, extrăgând informații esențiale precum metoda (GET, POST etc.), URL-ul, anteturile și corpul cererii. Acest proces transformă cererea brută într-un format structurat și ușor de procesat. Este crucial pentru înțelegerea și manipularea corectă a cererilor.

## 4.3 Router
  Routerul mapează URL-urile și metodele HTTP la funcțiile (handler-ele) care trebuie să gestioneze fiecare cerere. Pe baza rutei și metodei, routerul direcționează cererea către handler-ul corespunzător.

## 4.4 Generator de raspunsuri
  Generatorul de răspunsuri construiește răspunsul HTTP, incluzând statusul (ex. 200, 404), anteturile și corpul răspunsului. Aceasta asigură formatarea corectă a răspunsului înainte de a fi trimis către client. Permite gestionarea uniformă a structurii răspunsurilor și a codurilor de stare.

## 4.5 Serializator/Deserializator JSON
  Serializatorul transformă datele din structurile interne ale serverului în format JSON pentru a fi trimise la client, iar deserializatorul face conversia inversă. Aceasta permite schimbul facil de date structurate între client și server.

## 4.6 Manager de erori
  Managerul de erori gestionează și tratează erorile care apar în timpul procesării cererilor, oferind răspunsuri clare și coduri de eroare adecvate. Acesta asigură stabilitatea aplicației și ajută la diagnosticarea problemelor. Loghează erorile pentru a facilita depanarea și îmbunătățește experiența utilizatorului.

## 4.7 Middleware
  Middleware-urile sunt componente intermediare care procesează cererile înainte de a ajunge la router sau răspunsurile înainte de a fi trimise. Ele pot efectua autentificare, logare, validare și alte prelucrări necesare. Middleware-urile adaugă funcționalități suplimentare și modularitate în procesarea cererilor și răspunsurilor.