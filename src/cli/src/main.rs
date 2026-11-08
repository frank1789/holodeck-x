use std::env;
use std::fs::File;
use std::path::PathBuf;
use std::process;
use std::time::{SystemTime, UNIX_EPOCH};

mod directories;
use directories::get_home_dir;

use clap::{Parser, Subcommand};

use parsers::prelude::*;
use spdlog::prelude::*;

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    /// Input file to process
    #[arg(short, long, value_name = "FILE")]
    build: PathBuf,
}
#[derive(Subcommand, Debug, Clone)]
enum Commands {}

fn main() {
    spdlog::default_logger().set_level_filter(spdlog::LevelFilter::All);
    let args = Args::parse();
    let value = args.build;

    // point to a OS temporary directory
    let temp_path = env::temp_dir();

    // create a temporary file command
    let timestamp = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap()
        .as_nanos();
    let filename = format!("holodeck_{}.fbd", timestamp);
    let cmd_file_fbd = temp_path.join(filename);
    info!("temporary file path: {}", temp_path.display());
    info!("temporary filename {}", cmd_file_fbd.display());
    info!("home directory {}", get_home_dir().unwrap().display());

    println!("Parsed value {:?}", value);
    info!("ops");
    error!("something wrong");
    process::exit(0);
}
