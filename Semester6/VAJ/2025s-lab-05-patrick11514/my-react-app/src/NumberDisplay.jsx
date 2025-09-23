/**
 * @param {{number: number}} props
 */
export default function NumberDisplay(props) {
    const { number } = props;

    return <p>{number > 10 ? 'Number is big boi' : 'Number is too smol :('}</p>;
}
