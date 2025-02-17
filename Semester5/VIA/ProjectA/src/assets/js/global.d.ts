declare global {
    const Swal: typeof import('sweetalert2').default;
    const Chart: typeof import('chart.js/auto').Chart;

    type ChartOptions = import('chart.js').ChartOptions;
}

export { };
