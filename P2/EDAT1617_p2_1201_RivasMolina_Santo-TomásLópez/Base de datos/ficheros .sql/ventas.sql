-- Table: public.ventas

-- DROP TABLE public.ventas;

CREATE TABLE public.ventas
(
  fecha date NOT NULL,
  oferta integer,
  fidelizado integer,
  edicion text NOT NULL,
  modo_pago text,
  num_venta integer NOT NULL,
  precio_final money,
  CONSTRAINT ventas_pkey PRIMARY KEY (num_venta, edicion)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.ventas
  OWNER TO alumnodb;
