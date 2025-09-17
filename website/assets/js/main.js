// LOOM Main JavaScript

document.addEventListener('DOMContentLoaded', function() {
    // Mobile navigation toggle
    const navToggle = document.querySelector('.nav-toggle');
    const navMenu = document.querySelector('.nav-menu');

    if (navToggle) {
        navToggle.addEventListener('click', function() {
            navMenu.classList.toggle('active');
        });
    }

    // Copy code blocks to clipboard
    const codeBlocks = document.querySelectorAll('pre');
    codeBlocks.forEach(block => {
        const button = document.createElement('button');
        button.className = 'copy-button';
        button.textContent = 'Copy';
        button.style.position = 'absolute';
        button.style.top = '0.5rem';
        button.style.right = '0.5rem';
        button.style.padding = '0.25rem 0.5rem';
        button.style.fontSize = '0.875rem';
        button.style.backgroundColor = 'var(--primary-color)';
        button.style.color = 'white';
        button.style.border = 'none';
        button.style.borderRadius = '0.25rem';
        button.style.cursor = 'pointer';

        block.style.position = 'relative';
        block.appendChild(button);

        button.addEventListener('click', async () => {
            const code = block.querySelector('code').textContent;
            await navigator.clipboard.writeText(code);
            button.textContent = 'Copied!';
            setTimeout(() => {
                button.textContent = 'Copy';
            }, 2000);
        });
    });

    // Smooth scroll for anchor links
    document.querySelectorAll('a[href^="#"]').forEach(anchor => {
        anchor.addEventListener('click', function(e) {
            e.preventDefault();
            const target = document.querySelector(this.getAttribute('href'));
            if (target) {
                target.scrollIntoView({
                    behavior: 'smooth',
                    block: 'start'
                });
            }
        });
    });

    // Add anchor links to headers
    const headers = document.querySelectorAll('h2, h3, h4');
    headers.forEach(header => {
        if (header.id) {
            const link = document.createElement('a');
            link.href = '#' + header.id;
            link.className = 'header-link';
            link.innerHTML = '🔗';
            link.style.marginLeft = '0.5rem';
            link.style.opacity = '0.3';
            link.style.fontSize = '0.8em';
            link.style.textDecoration = 'none';

            header.appendChild(link);

            header.addEventListener('mouseenter', () => {
                link.style.opacity = '1';
            });

            header.addEventListener('mouseleave', () => {
                link.style.opacity = '0.3';
            });
        }
    });

    // MathJax configuration
    window.MathJax = {
        tex: {
            inlineMath: [['$', '$'], ['\\(', '\\)']],
            displayMath: [['$$', '$$'], ['\\[', '\\]']],
            processEscapes: true,
            processEnvironments: true
        },
        options: {
            skipHtmlTags: ['script', 'noscript', 'style', 'textarea', 'pre', 'code']
        }
    };

    // Search functionality (basic implementation)
    const searchInput = document.querySelector('#search-input');
    if (searchInput) {
        searchInput.addEventListener('input', function(e) {
            const query = e.target.value.toLowerCase();
            const searchResults = document.querySelector('#search-results');

            if (query.length < 2) {
                searchResults.innerHTML = '';
                return;
            }

            // This would need to be populated with actual content
            // For now, it's a placeholder
            searchResults.innerHTML = '<p>Searching for: ' + query + '</p>';
        });
    }
});

// Playground functionality
if (document.querySelector('.playground-container')) {
    const editor = document.querySelector('#loom-editor');
    const output = document.querySelector('#output-display');
    const runButton = document.querySelector('#run-button');

    if (runButton) {
        runButton.addEventListener('click', function() {
            const code = editor.value;
            output.textContent = 'Processing LOOM code...\n\n';

            // Simulate processing (actual implementation would connect to backend)
            setTimeout(() => {
                output.textContent += 'Note: LOOM runtime not yet implemented.\n';
                output.textContent += 'Your code:\n\n' + code;
            }, 500);
        });
    }

    // Load example code
    const exampleSelect = document.querySelector('#example-select');
    if (exampleSelect) {
        const examples = {
            hello: `// Hello consciousness
weave mind {
    <~> curiosity: 0.8
    <~> awareness: 0.7
}

:topology`,
            learning: `// Learning system
weave learner {
    <~> input_pattern
    <~> memory: 0.5
}

evolve learn {
    when: coactivation > 0.7,
    strengthen: input_pattern <~> memory
}

:hebbian`,
            emotional: `// Emotional dynamics
weave emotions {
    <~> joy: 0.6
    <~> fear: 0.3
    <~> curiosity: 0.8
}

context {
    stress: 0.4
    modulates {
        ¥emotional_spreading¥: amplify when stress > 0.5
    }
}`
        };

        exampleSelect.addEventListener('change', function(e) {
            if (examples[e.target.value]) {
                editor.value = examples[e.target.value];
            }
        });
    }
}