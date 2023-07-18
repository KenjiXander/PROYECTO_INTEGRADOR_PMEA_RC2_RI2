#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTOS 100
#define MAX_USUARIOS 3

struct Producto {
    char nombre[50];
    int cantidad;
    float precio;
    float iva;
};

struct Inventario {
    struct Producto productos[MAX_PRODUCTOS];
    int num_productos;
};

struct Usuario {
    char nombre[50];
    char contrasena[50];
    char rol[50];
};

// Definimos las funciones que se van a usar a lo largo del codigo
void ingresarProducto(struct Inventario *inventario, struct Usuario usuario);
void editarProducto(struct Inventario *inventario, struct Usuario usuario);
void eliminarProducto(struct Inventario *inventario, struct Usuario usuario);
void listarProductos(struct Inventario *inventario);

void guardarInventario(struct Inventario *inventario);
void cargarInventario(struct Inventario *inventario);

int main() {
    struct Inventario inventario;
    inventario.num_productos = 0;

    struct Usuario usuarios[MAX_USUARIOS] = { // Definir los usuarios que se pueden usar
        { "Admin", "admin", "administrador" },
        { "Empleado", "12345", "empleado" },
        { "Cliente", "soycliente", "cliente" }
    };

    cargarInventario(&inventario);

    int opcion;
    char nombre[50];
    char contrasena[50];

    printf("Ingrese su nombre de usuario: ");
    scanf("%s", nombre);
    printf("Ingrese su contraseña: ");
    scanf("%s", contrasena);

    int usuarioValido = autenticarUsuario(usuarios, nombre, contrasena);
    if (usuarioValido == -1) {
        printf("Usuario o contraseña incorrectos. Saliendo del programa...\n");
        return 0;
    }

    struct Usuario usuario = usuarios[usuarioValido];

    do { // Menu del programa
        printf("\nSistema de inventarios\n");
        printf("1. Ingresar producto\n");
        printf("2. Editar producto\n");
        printf("3. Eliminar producto\n");
        printf("4. Listar productos\n");
        printf("5. Salir\n");
        printf("Ingrese una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                if (strcmp(usuario.rol, "administrador") == 0 || strcmp(usuario.rol, "empleado") == 0) {
                    ingresarProducto(&inventario, usuario);
                    guardarInventario(&inventario);
                } else {
                    printf("Acceso no autorizado. Permiso requerido: 'administrador' o 'empleado'.\n");
                }
                break;
            case 2:
                if (strcmp(usuario.rol, "administrador") == 0) {
                    editarProducto(&inventario, usuario);
                    guardarInventario(&inventario);
                } else {
                    printf("Acceso no autorizado. Permiso requerido: 'administrador'.\n");
                }
                break;
            case 3:
                if (strcmp(usuario.rol, "administrador") == 0 || strcmp(usuario.rol, "empleado") == 0) {
                    eliminarProducto(&inventario, usuario);
                    guardarInventario(&inventario);
                } else {
                    printf("Acceso no autorizado. Permiso requerido: 'administrador' o 'empleado'.\n");
                }
                break;
            case 4:
                listarProductos(&inventario);
                break;
            case 5:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción inválida. Intente de nuevo.\n");
        }
    } while (opcion != 5);

    return 0;
}

void ingresarProducto(struct Inventario *inventario, struct Usuario usuario) { // Funcion del codigo para ingresar los productos
    if (inventario->num_productos == MAX_PRODUCTOS) {
        printf("No se pueden ingresar más productos.\n");
        return;
    }

    struct Producto producto;
    printf("Ingrese el nombre del producto: ");
    getchar(); 
    fgets(producto.nombre, sizeof(producto.nombre), stdin);
    producto.nombre[strcspn(producto.nombre, "\n")] = '\0';


    int indice = buscarProducto(inventario, producto.nombre);
    if (indice != -1) {
        printf("El producto ya existe. Se actualizarán los datos.\n");
        printf("Ingrese la nueva cantidad del producto: ");
        scanf("%d", &inventario->productos[indice].cantidad);
        printf("Ingrese el nuevo precio del producto: ");
        scanf("%f", &inventario->productos[indice].precio);
        printf("Ingrese el nuevo IVA del producto (en decimal): ");
        scanf("%f", &inventario->productos[indice].iva);
    } else {
        printf("Ingrese la cantidad del producto: ");
        scanf("%d", &producto.cantidad);
        printf("Ingrese el precio del producto: ");
        scanf("%f", &producto.precio);
        printf("Ingrese el IVA del producto (en decimal): ");
        scanf("%f", &producto.iva);

        inventario->productos[inventario->num_productos] = producto;
        inventario->num_productos++;
    }

    printf("Producto ingresado exitosamente.\n");
}

void editarProducto(struct Inventario *inventario, struct Usuario usuario) { // Funcion del codigo para editar los productos ingresados
    if (strcmp(usuario.rol, "administrador") != 0) {
        printf("Acceso no autorizado. Permiso requerido: 'administrador'.\n");
        return;
    }

    if (inventario->num_productos == 0) {
        printf("No hay productos ingresados.\n");
        return;
    }

    char nombre[50];
    printf("Ingrese el nombre del producto a editar: ");
    getchar(); 
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0'; 

    int indice = buscarProducto(inventario, nombre);
    if (indice == -1) {
        printf("Producto no encontrado.\n");
    } else {
        printf("Ingrese la nueva cantidad del producto: ");
        scanf("%d", &inventario->productos[indice].cantidad);
        printf("Ingrese el nuevo precio del producto: ");
        scanf("%f", &inventario->productos[indice].precio);
        printf("Ingrese el nuevo IVA del producto (en decimal): ");
        scanf("%f", &inventario->productos[indice].iva);

        printf("Producto editado exitosamente.\n");
    }
}


void eliminarProducto(struct Inventario *inventario, struct Usuario usuario) { // Funcion del codigo para eliminar los productos ingresados
    if (strcmp(usuario.rol, "administrador") != 0) {
        printf("Acceso no autorizado. Permiso requerido: 'administrador'.\n");
        return;
    }

    if (inventario->num_productos == 0) {
        printf("No hay productos ingresados.\n");
        return;
    }

    char nombre[50];
    printf("Ingrese el nombre del producto a eliminar: ");
    getchar();
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    int indice = buscarProducto(inventario, nombre);
    if (indice == 0) {
        printf("Producto no encontrado.\n");
    } else {
        for (int i = indice; i < inventario->num_productos - 1; i++) {
            inventario->productos[i] = inventario->productos[i + 1];
        }
        inventario->num_productos--;

        printf("Producto eliminado exitosamente.\n");
    }
}



void listarProductos(struct Inventario *inventario) { // Funcion del codigo para listar los productos ya ingresados
    if (inventario->num_productos == 0) {
        printf("No hay productos ingresados.\n");
        return;
    }

    printf("\nProductos ingresados:\n");
    printf("---------------------\n");
    for (int i = 0; i < inventario->num_productos; i++) {
        float precio_sin_iva = inventario->productos[i].precio;
        // float precio_total = precio_sin_iva * (1 + inventario->productos[i].iva) * inventario->productos[i].cantidad;
        float precio_total = precio_sin_iva + (precio_sin_iva * inventario->productos[i].iva);
        printf("Nombre: %s\nCantidad: %d\nPrecio: %.2f\nIVA: %.2f\nPrecio Total: %.2f\n", inventario->productos[i].nombre, inventario->productos[i].cantidad, precio_sin_iva, inventario->productos[i].iva, precio_total);
        printf("---------------------\n");
    }
}

int autenticarUsuario(struct Usuario *usuarios, const char *nombre, const char *contrasena) { // Funcion del codigo para autenticar los usuarios
    for (int i = 0; i < MAX_USUARIOS; i++) {
        if (strcmp(usuarios[i].nombre, nombre) == 0 && strcmp(usuarios[i].contrasena, contrasena) == 0) {
            return i;
        }
    }
    return -1; // Usamos el "Return -1" para indicar que el usuario no fue encontrado
}

int buscarProducto(struct Inventario *inventario, const char *nombre) { // Funcion del codigo para buscar los productos ingresados
    for (int i = 0; i < inventario->num_productos; i++) {
        if (strcmp(inventario->productos[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1; // Usamos el "Return -1" para indicar que el producto no fue encontrado
}

void guardarInventario(struct Inventario *inventario) { // Funcion del codigo para almacenar los productos ingresados en el archivo "datos.txt"
    FILE *archivo = fopen("datos.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    fprintf(archivo, "%d\n", inventario->num_productos);
    for (int i = 0; i < inventario->num_productos; i++) {
        fprintf(archivo, "%s %d %.2f %.2f\n", inventario->productos[i].nombre, inventario->productos[i].cantidad, inventario->productos[i].precio, inventario->productos[i].iva);
    }

    fclose(archivo);
}

void cargarInventario(struct Inventario *inventario) { // Funcion del codigo para abrir la informacion de los productos almacenados en el archivo "datos.txt"
    FILE *archivo = fopen("datos.txt", "r");
    if (archivo == NULL) {
        printf("No se encontró el archivo de datos. Se creará uno nuevo.\n");
        return;
    }

    fscanf(archivo, "%d", &(inventario->num_productos));
    for (int i = 0; i < inventario->num_productos; i++) {
        fscanf(archivo, "%s %d %f %f", inventario->productos[i].nombre, &(inventario->productos[i].cantidad), &(inventario->productos[i].precio), &(inventario->productos[i].iva));
    }

    fclose(archivo);
}
