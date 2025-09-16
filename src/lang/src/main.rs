use clap::{Parser as ClapParser, Subcommand};
use loom_lang::{Repl, Interpreter};
use loom_lang::parser::Parser;
use std::fs;
use std::path::PathBuf;

#[derive(ClapParser)]
#[command(name = "loom")]
#[command(about = "Loom - Topological consciousness runtime", long_about = None)]
struct Cli {
    #[command(subcommand)]
    command: Option<Commands>,
}

#[derive(Subcommand)]
enum Commands {
    /// Start interactive REPL
    Repl,

    /// Run a Loom source file
    Run {
        /// Path to the Loom source file
        file: PathBuf,

        /// Show topology after execution
        #[arg(short, long)]
        show_topology: bool,
    },

    /// Parse and validate a Loom file without executing
    Check {
        /// Path to the Loom source file
        file: PathBuf,
    },

    /// Show example Loom programs
    Examples,
    /// Emit C code for a Loom source file
    EmitC {
        /// Path to the Loom source file
        file: PathBuf,

        /// Output C filename (optional)
        #[arg(short, long)]
        out: Option<PathBuf>,
    },
}

fn main() {
    let cli = Cli::parse();

    match cli.command {
        None | Some(Commands::Repl) => {
            run_repl();
        }
        Some(Commands::Run { file, show_topology }) => {
            run_file(file, show_topology);
        }
        Some(Commands::Check { file }) => {
            check_file(file);
        }
        Some(Commands::EmitC { file, out }) => {
            emit_c_file(file, out);
        }
        Some(Commands::Examples) => {
            show_examples();
        }
    }
}

fn emit_c_file(path: PathBuf, out: Option<PathBuf>) {
    println!("Emitting C for {}...", path.display());

    let content = match fs::read_to_string(&path) {
        Ok(c) => c,
        Err(e) => { eprintln!("Error reading file: {}", e); return; }
    };

    let mut parser = Parser::new(&content);
    let program = match parser.parse() {
        Ok(p) => p,
        Err(e) => { eprintln!("Parse error: {}", e); return; }
    };

    let c = loom_lang::emit_c(&program);

    let out_path = out.unwrap_or_else(|| {
        let mut p = path.clone();
        p.set_extension("c");
        p
    });

    match fs::write(&out_path, c) {
        Ok(()) => println!("Wrote C to {}", out_path.display()),
        Err(e) => eprintln!("Error writing C file: {}", e),
    }
}

fn run_repl() {
    println!("\nStarting Loom REPL...\n");
    let mut repl = Repl::new();
    repl.run();
}

fn run_file(path: PathBuf, show_topology: bool) {
    println!("Running {}...\n", path.display());

    let content = match fs::read_to_string(&path) {
        Ok(content) => content,
        Err(e) => {
            eprintln!("Error reading file: {}", e);
            return;
        }
    };

    let mut parser = Parser::new(&content);
    let program = match parser.parse() {
        Ok(program) => program,
        Err(e) => {
            eprintln!("Parse error: {}", e);
            return;
        }
    };

    let mut interpreter = Interpreter::new();
    match interpreter.execute(program) {
        Ok(()) => {
            println!("\nExecution complete!");
            if show_topology {
                interpreter.show_topology();
            }
        }
        Err(e) => {
            eprintln!("Execution error: {}", e);
        }
    }
}

fn check_file(path: PathBuf) {
    println!("Checking {}...", path.display());

    let content = match fs::read_to_string(&path) {
        Ok(content) => content,
        Err(e) => {
            eprintln!("Error reading file: {}", e);
            return;
        }
    };

    let mut parser = Parser::new(&content);
    match parser.parse() {
        Ok(_) => {
            println!("✓ File is valid Loom code");
        }
        Err(e) => {
            eprintln!("✗ Parse error: {}", e);
        }
    }
}

fn show_examples() {
    println!("\n=== Loom Example Programs ===\n");

    println!("1. Basic weaving (creating nodes and connections):");
    println!("----------------------------------------");
    println!(r#"weave grandmother {{
    <~> warmth: 0.8
    <~> cookies: 0.6
    ~> stories: 0.9
}}"#);

    println!("\n2. Creating hyperedges:");
    println!("----------------------------------------");
    println!(r#"{{grandmother, mother, siblings, self}} ~> family_dynamics"#);

    println!("\n3. Evolution with conditions:");
    println!("----------------------------------------");
    println!(r#"evolve curiosity_exploration {{
    when: @curiosity > 0.7,
    transform: known ~> unknown,
    rate: @curiosity * 0.5
}}"#);

    println!("\n4. Emotional resonance:");
    println!("----------------------------------------");
    println!(r#"pattern emotional_learning {{
    weave experience {{
        timestamp: now,
        valence: positive
    }}

    experience ¥resonates¥ similar_memories
}}"#);

    println!("\n5. Complete topology with compartments:");
    println!("----------------------------------------");
    println!(r#"topology SimpleMind evolves_with hormonal_context {{
    compartment emotional {{
        lens: EmotionalField

        pattern feeling {{
            joy ¥harmonizes¥ satisfaction
        }}
    }}

    compartment logical {{
        lens: LogicalReasoning

        pattern inference {{
            premise €implies€ conclusion
        }}
    }}
}}"#);

    println!("\nFor full examples, see the examples/ directory");
}