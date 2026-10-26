use std::process;

use clap::Parser;


#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    #[arg(short, long, default_value_t = 25)]
    value : i32,
}


fn main()
{
    println!("Hello World!");
    let args = Args::parse();
    let value = args.value;
    println!("Parsed value {}", value);
    process::exit(0);
}
