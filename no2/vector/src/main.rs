use std::io;

fn main() {
    let (rows, cols) = get_dimensions();
    let matrix1 = read_matrix(rows, cols, "첫 번째");
    let matrix2 = read_matrix(rows, cols, "두 번째");
    let result = add_matrices(&matrix1, &matrix2);

    println!("두 행렬을 더한 결과는 다음과 같습니다:");
    print_matrix(&result);
}

fn get_dimensions() -> (usize, usize) {
    println!("행과 열의 개수를 입력하세요 (예: 2 3):");
    let mut input = String::new();
    io::stdin().read_line(&mut input).unwrap();
    let dims: Vec<usize> = input
        .trim()
        .split_whitespace()
        .map(|x| x.parse().unwrap())
        .collect();
    (dims[0], dims[1])
}

fn read_matrix(rows: usize, cols: usize, name: &str) -> Vec<Vec<i32>> {
    println!("{} 행렬의 원소를 행별로 입력하세요:", name);
    (0..rows)
        .map(|_| {
            let mut input = String::new();
            io::stdin().read_line(&mut input).unwrap();
            input
                .trim()
                .split_whitespace()
                .map(|x| x.parse::<i32>().unwrap())
                .collect::<Vec<i32>>()
        })
        .collect()
}

fn add_matrices(matrix1: &Vec<Vec<i32>>, matrix2: &Vec<Vec<i32>>) -> Vec<Vec<i32>> {
    matrix1
        .iter()
        .zip(matrix2.iter())
        .map(|(row1, row2)| row1.iter().zip(row2).map(|(a, b)| a + b).collect())
        .collect()
}

fn print_matrix(matrix: &Vec<Vec<i32>>) {
    for row in matrix {
        println!("{}", row.iter().map(|x| x.to_string()).collect::<Vec<_>>().join(" "));
    }
}
