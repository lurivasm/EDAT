﻿
CREATE TABLE Libro (
	Id_libro int PRIMARY KEY,
	Titulo_original text NOT NULL
);

CREATE TABLE Autor(
	Nombre text NOT NULL,
	Id_autor int PRIMARY KEY
);

CREATE TABLE Relacion_AL(
	Libro int REFERENCES Libro(Id_libro),
	Autor int REFERENCES Autor(Id_autor),
	PRIMARY KEY(Libro,Autor)
);

CREATE TABLE Edicion(
	ISBN int PRIMARY KEY,
	Titulo text NOT NULL,
	Id int REFERENCES Libro(Id_libro),
	Tapa text,
	Idioma text,
	Tamaño text,
	Precio float NOT NULL,
	Editorial text NOT NULL
);

CREATE TABLE Oferta(
	Num_oferta int PRIMARY KEY,
	Descuento int,
	Fecha_inicio date,
	Fecha_fin date,
	Libro int REFERENCES Libro(Id_libro)
);

CREATE TABLE Fidelizados(
	Id_fidelizado int PRIMARY KEY,
	Tarjeta int UNIQUE NOT NULL,
	Gastos float 
);

CREATE TABLE Ventas(
	Num_venta int PRIMARY KEY,
	Modo_pago text NOT NULL,
	Fecha date NOT NULL,
	Edicion int REFERENCES Edicion(ISBN),
	Precio_final float NOT NULL,
	Oferta int REFERENCES Oferta(Num_oferta),
	Fidelizado int REFERENCES Fidelizados(Id_fidelizado)
);
